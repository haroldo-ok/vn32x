import sys, os

import pyximport
pyximport.install()

renpy_home = 'C:/Util/renpy-6.99.10-sdk'
sys.path.append(renpy_home)

def main ():
    import renpy
    import renpy.object
    import renpy.exports
    import renpy.config
    import renpy.parser
    renpy.parser.parse(r'C:\Util\renpy-6.99.10-sdk\the_question\game\script.rpy')


main()