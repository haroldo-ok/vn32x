import unittest
import rpy_parser, textwrap

class TestParser(unittest.TestCase):

    def test_code(self):
        code = r"""
        # Declare images used by this game.
        image bg lecturehall = "lecturehall.jpg"
        image bg uni = "uni.jpg"
        define s = Character("Sylvie")
        define s = Character("Sylvie", color="#c8ffc8")
        define s = Character('Sylvie', color="#c8ffc8")
        #define m = Character('Me', color="#c8c8ff")

        # The game starts here.
        label start:
            scene bg lecturehall
            "Well, professor Eileen's lecture was interesting."
            "Sylvie" "Oh, hi, do we walk home together?"
            m "Yes..."
            menu:
                "It's a story with pictures.":
                    "Text inside a menu option."
                    jump vn

                "It's a hentai game.":
                    jump hentai

        label another:
            "This is a test."

        #Comment here
        """

        tree = rpy_parser.GardenSnakeParser().parse(textwrap.dedent(code))
        print(tree)


if __name__ == '__main__':
    unittest.main()
