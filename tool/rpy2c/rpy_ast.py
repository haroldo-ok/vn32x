class RpyScript(object):
    def __init__(self, declarations, labels):
        self.declarations = declarations
        self.labels = labels

    def __repr__(self):
        return "RpyScript {declarations} {labels}".format(**self.__dict__)


class ImageDecl(object):
    def __init__(self, name, state, image):
        self.name = name
        self.state = state
        self.image = image

    def __repr__(self):
        return "Image {name} {state} {image}".format(**self.__dict__)


class CharacterDecl(object):
    def __init__(self, name, char_name, params):
        self.name = name
        self.char_name = char_name
        self.params = params

    def __repr__(self):
        return "Character {name} {char_name} {params}".format(**self.__dict__)


class CharacterRef(object):
    def __init__(self, name):
        self.name = name

    def __repr__(self):
        return "Ref {name}".format(**self.__dict__)


class Label(object):
    def __init__(self, name, commands):
        self.name = name
        self.commands = commands

    def __repr__(self):
        return "Label {name} {commands}".format(**self.__dict__)


class SceneCmd(object):
    def __init__(self, name, state):
        self.name = name
        self.state = state

    def __repr__(self):
        return "Scene {name} {state}".format(**self.__dict__)


class SayCmd(object):
    def __init__(self, character, text):
        self.character = character
        self.text = text

    def __repr__(self):
        return "Say {character} {text}".format(**self.__dict__)


class MenuCmd(object):
    def __init__(self, options):
        self.options = options

    def __repr__(self):
        return "Menu {options}".format(**self.__dict__)


class MenuOpt(object):
    def __init__(self, text, commands):
        self.text = text
        self.commands = commands

    def __repr__(self):
        return "Opt {text} {commands}".format(**self.__dict__)


class JumpCmd(object):
    def __init__(self, label):
        self.label = label

    def __repr__(self):
        return "Jump {label}".format(**self.__dict__)
