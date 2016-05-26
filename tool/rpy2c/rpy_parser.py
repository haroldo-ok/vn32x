print r'C:\Util\renpy-6.99.10-sdk\the_question\game\script.rpy'

import sys
sys.path.insert(0,"../..")
from ply import *

##### Lexer ######
#import lex
import decimal

tokens = (
    'IMAGE',
    'DEFINE',
    'CHARACTER',
    'LABEL',
    'SCENE',
    'MENU',
    'JUMP',
    'DEF',
    'IF',
    'NAME',
    'NUMBER',  # Python decimals
    'STRING',  # single quoted strings only; syntax of raw strings
    'LPAR',
    'RPAR',
    'COLON',
    'EQ',
    'ASSIGN',
    'LT',
    'GT',
    'PLUS',
    'MINUS',
    'MULT',
    'DIV',
    'RETURN',
    'WS',
    'NEWLINE',
    'COMMA',
    'SEMICOLON',
    'INDENT',
    'DEDENT',
    'ENDMARKER',
    )

#t_NUMBER = r'\d+'
# taken from decmial.py but without the leading sign
def t_NUMBER(t):
    r"""(\d+(\.\d*)?|\.\d+)([eE][-+]? \d+)?"""
    t.value = decimal.Decimal(t.value)
    return t

def t_STRING(t):
    r'("([^\\"]+|\\"|\\\\)*"|\'([^\\\']+|\\\'|\\\\)*\')'  # I think this is right ...
    t.value=t.value[1:-1].decode("string-escape") # .swapcase() # for fun
    return t

t_COLON = r':'
t_EQ = r'=='
t_ASSIGN = r'='
t_LT = r'<'
t_GT = r'>'
t_PLUS = r'\+'
t_MINUS = r'-'
t_MULT = r'\*'
t_DIV = r'/'
t_COMMA = r','
t_SEMICOLON = r';'

# Ply nicely documented how to do this.

RESERVED = {
  "image": "IMAGE",
  "define": "DEFINE",
  "Character": "CHARACTER",
  "label": "LABEL",
  "scene": "SCENE",
  "menu": "MENU",
  "jump": "JUMP",
  "def": "DEF",
  "if": "IF",
  "return": "RETURN",
  }

def t_NAME(t):
    r'[a-zA-Z_][a-zA-Z0-9_]*'
    t.type = RESERVED.get(t.value, "NAME")
    return t

# Putting this before t_WS let it consume lines with only comments in
# them so the latter code never sees the WS part.  Not consuming the
# newline.  Needed for "if 1: #comment"
def t_comment(t):
    r"[ ]*\043[^\n]*"  # \043 is '#'
    pass


# Whitespace
def t_WS(t):
    r' [ ]+ '
    if t.lexer.at_line_start and t.lexer.paren_count == 0:
        return t

# Don't generate newline tokens when inside of parenthesis, eg
#   a = (1,
#        2, 3)
def t_newline(t):
    r'\n+'
    t.lexer.lineno += len(t.value)
    t.type = "NEWLINE"
    if t.lexer.paren_count == 0:
        return t

def t_LPAR(t):
    r'\('
    t.lexer.paren_count += 1
    return t

def t_RPAR(t):
    r'\)'
    # check for underflow?  should be the job of the parser
    t.lexer.paren_count -= 1
    return t


def t_error(t):
    raise SyntaxError("Unknown symbol %r" % (t.value[0],))
    print "Skipping", repr(t.value[0])
    t.lexer.skip(1)

## I implemented INDENT / DEDENT generation as a post-processing filter

# The original lex token stream contains WS and NEWLINE characters.
# WS will only occur before any other tokens on a line.

# I have three filters.  One tags tokens by adding two attributes.
# "must_indent" is True if the token must be indented from the
# previous code.  The other is "at_line_start" which is True for WS
# and the first non-WS/non-NEWLINE on a line.  It flags the check so
# see if the new line has changed indication level.

# Python's syntax has three INDENT states
#  0) no colon hence no need to indent
#  1) "if 1: go()" - simple statements have a COLON but no need for an indent
#  2) "if 1:\n  go()" - complex statements have a COLON NEWLINE and must indent
NO_INDENT = 0
MAY_INDENT = 1
MUST_INDENT = 2

# only care about whitespace at the start of a line
def track_tokens_filter(lexer, tokens):
    lexer.at_line_start = at_line_start = True
    indent = NO_INDENT
    saw_colon = False
    for token in tokens:
        token.at_line_start = at_line_start

        if token.type == "COLON":
            at_line_start = False
            indent = MAY_INDENT
            token.must_indent = False

        elif token.type == "NEWLINE":
            at_line_start = True
            if indent == MAY_INDENT:
                indent = MUST_INDENT
            token.must_indent = False

        elif token.type == "WS":
            assert token.at_line_start == True
            at_line_start = True
            token.must_indent = False

        else:
            # A real token; only indent after COLON NEWLINE
            if indent == MUST_INDENT:
                token.must_indent = True
            else:
                token.must_indent = False
            at_line_start = False
            indent = NO_INDENT

        yield token
        lexer.at_line_start = at_line_start

def _new_token(type, lineno):
    tok = lex.LexToken()
    tok.type = type
    tok.value = None
    tok.lineno = lineno
    return tok

# Synthesize a DEDENT tag
def DEDENT(lineno):
    return _new_token("DEDENT", lineno)

# Synthesize an INDENT tag
def INDENT(lineno):
    return _new_token("INDENT", lineno)


# Track the indentation level and emit the right INDENT / DEDENT events.
def indentation_filter(tokens):
    # A stack of indentation levels; will never pop item 0
    levels = [0]
    token = None
    depth = 0
    prev_was_ws = False
    for token in tokens:
##        if 1:
##            print "Process", token,
##            if token.at_line_start:
##                print "at_line_start",
##            if token.must_indent:
##                print "must_indent",
##            print

        # WS only occurs at the start of the line
        # There may be WS followed by NEWLINE so
        # only track the depth here.  Don't indent/dedent
        # until there's something real.
        if token.type == "WS":
            assert depth == 0
            depth = len(token.value)
            prev_was_ws = True
            # WS tokens are never passed to the parser
            continue

        if token.type == "NEWLINE":
            depth = 0
            if prev_was_ws or token.at_line_start:
                # ignore blank lines
                continue
            # pass the other cases on through
            yield token
            continue

        # then it must be a real token (not WS, not NEWLINE)
        # which can affect the indentation level

        prev_was_ws = False
        if token.must_indent:
            # The current depth must be larger than the previous level
            if depth > levels[-1]:
                levels.append(depth)
                yield INDENT(token.lineno)
            else:
                while levels and depth < levels[-1]:
                    yield DEDENT(token.lineno)
                    levels.pop()

        elif token.at_line_start:
            # Must be on the same level or one of the previous levels
            if depth == levels[-1]:
                # At the same level
                pass
            elif depth > levels[-1]:
                raise IndentationError("indentation increase but not in new block")
            else:
                # Back up; but only if it matches a previous level
                try:
                    i = levels.index(depth)
                except ValueError:
                    raise IndentationError("inconsistent indentation")
                for _ in range(i+1, len(levels)):
                    yield DEDENT(token.lineno)
                    levels.pop()

        yield token

    ### Finished processing ###

    # Must dedent any remaining levels
    if len(levels) > 1:
        assert token is not None
        for _ in range(1, len(levels)):
            yield DEDENT(token.lineno)


# The top-level filter adds an ENDMARKER, if requested.
# Python's grammar uses it.
def filter(lexer, add_endmarker = True):
    token = None
    tokens = iter(lexer.token, None)
    tokens = track_tokens_filter(lexer, tokens)
    for token in indentation_filter(tokens):
        yield token

    if add_endmarker:
        lineno = 1
        if token is not None:
            lineno = token.lineno
        yield _new_token("ENDMARKER", lineno)

# Combine Ply and my filters into a new lexer

class IndentLexer(object):
    def __init__(self, debug=0, optimize=0, lextab='lextab', reflags=0):
        self.lexer = lex.lex(debug=debug, optimize=optimize, lextab=lextab, reflags=reflags)
        self.token_stream = None
    def input(self, s, add_endmarker=True):
        self.lexer.paren_count = 0
        self.lexer.input(s)
        self.token_stream = filter(self.lexer, add_endmarker)
    def token(self):
        try:
            return self.token_stream.next()
        except StopIteration:
            return None

##########   Parser (tokens -> AST) ######

# also part of Ply
#import yacc

import collections
from rpy_ast import *

# I use the Python AST
from compiler import ast

# Helper function
def Assign(left, right):
    names = []
    if isinstance(left, ast.Name):
        # Single assignment on left
        return ast.Assign([ast.AssName(left.name, 'OP_ASSIGN')], right)
    elif isinstance(left, ast.Tuple):
        # List of things - make sure they are Name nodes
        names = []
        for child in left.getChildren():
            if not isinstance(child, ast.Name):
                raise SyntaxError("that assignment not supported")
            names.append(child.name)
        ass_list = [ast.AssName(name, 'OP_ASSIGN') for name in names]
        return ast.Assign([ast.AssTuple(ass_list)], right)
    else:
        raise SyntaxError("Can't do that yet")


# The grammar comments come from Python's Grammar/Grammar file

## NB: compound_stmt in single_input is followed by extra NEWLINE!
# file_input: (NEWLINE | stmt)* ENDMARKER
def p_file_input_end(p):
    """file_input_end : declarations labels ENDMARKER"""
    p[0] = RpyScript(p[1], p[2])


def p_declarations(p):
    """declarations : declarations NEWLINE
                    | declarations declaration
                    | NEWLINE
                    | declaration """
    p[0] = list(flatten(p[1:]))

def p_declaration(p):
    """declaration : image_decl
                   | character_decl  """
    p[0] = p[1]

def p_image_decl(p):
    """image_decl : IMAGE NAME NAME ASSIGN STRING NEWLINE
    """
    p[0] = ImageDecl(p[2], p[3], p[5])

def p_character_decl(p):
    """character_decl : DEFINE NAME ASSIGN CHARACTER LPAR STRING named_params RPAR NEWLINE
    """
    p[0] = CharacterDecl(p[2], p[6], p[7])

def p_named_params(p):
    """named_params : COMMA NAME ASSIGN STRING
                    | """
    p[0] = {p[2]: p[4]} if len(p) > 1 else {}


def p_labels(p):
    """labels : labels NEWLINE
              | labels label
              | NEWLINE
              | label """
    p[0] = list(flatten(p[1:]))

def p_label(p):
    """label : LABEL NAME COLON NEWLINE INDENT dialog_cmds DEDENT
    """
    p[0] = Label(p[2], p[6])

def p_dialog_cmds(p):
    """dialog_cmds : dialog_cmds NEWLINE
                   | dialog_cmds dialog_cmd
                   | NEWLINE
                   | dialog_cmd """
    p[0] = list(flatten(p[1:]))

def p_dialog_cmd(p):
    """dialog_cmd : scene_cmd
                  | say_cmd
                  | menu_cmd
                  | jump_cmd """
    p[0] = p[1]

def p_scene_cmd(p):
    """scene_cmd : SCENE NAME NAME NEWLINE"""
    p[0] = SceneCmd(p[2], p[3])

def p_say_cmd(p):
    """say_cmd : STRING NEWLINE
               | STRING STRING NEWLINE
               | character_ref STRING NEWLINE """
    if len(p) == 2:
        p[0] = SayCmd(None, p[1])
    else:
        p[0] = SayCmd(p[1], p[2])

def p_character_ref(p):
    """character_ref : NAME"""
    p[0] = CharacterRef(p[1])

def p_menu_cmd(p):
    """menu_cmd : MENU COLON NEWLINE INDENT menu_opts DEDENT"""
    p[0] = MenuCmd(p[5])

def p_menu_opts(p):
    """menu_opts : menu_opts NEWLINE
                 | menu_opts menu_opt
                 | NEWLINE
                 | menu_opt """
    p[0] = list(flatten(p[1:]))

def p_menu_opt(p):
    """menu_opt : STRING COLON NEWLINE INDENT dialog_cmds DEDENT
                | STRING COLON NEWLINE """
    p[0] = MenuOpt(p[1], p[5] if len(p) > 4 else None)

def p_jump_cmd(p):
    """jump_cmd : JUMP NAME NEWLINE"""
    p[0] = JumpCmd(p[2])



def p_error(p):
    #print "Error!", repr(p)
    raise SyntaxError(p)


class RpyParser(object):
    def __init__(self, lexer = None):
        if lexer is None:
            lexer = IndentLexer()
        self.lexer = lexer
        self.parser = yacc.yacc(start="file_input_end")

    def parse(self, code):
        self.lexer.input(code)
        result = self.parser.parse(lexer = self.lexer)
        return ast.Module(None, result)


def flatten(l):
    """Flattens a list of lists, which may be irregular.
    Based on http://stackoverflow.com/a/2158532/679240"""

    for el in l:
        if isinstance(el, collections.Iterable) and not isinstance(el, basestring):
            for sub in flatten(el):
                yield sub
        else:
            yield el

