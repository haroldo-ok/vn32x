import sys, os

sys.path.append(r'C:\Users\Haroldo\Projetos\32x\vn32x\tool')

def main ():
    #import renpy
    """
    import renpy.object
    import renpy.exports
    import renpy.config
    import renpy.game
    """
    import renpy.parser
    import renpy.game
    import renpy.execution

    context = renpy.execution.Context(False, None, clear=True)
    renpy.game.contexts.append(context)

    renpy.parser.parse(r'C:\Util\renpy-6.99.10-sdk\the_question\game\script.rpy')


main()
