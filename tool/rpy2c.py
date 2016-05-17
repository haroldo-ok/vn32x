import sys, os

#renpy_home = 'C:/Util/renpy-6.99.10-sdk'
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
    renpy.parser.parse(r'C:\Util\renpy-6.99.10-sdk\the_question\game\script.rpy')


main()