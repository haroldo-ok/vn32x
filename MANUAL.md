# VN32X Manual

VN32X uses a subset (in other words, not everything is supported) of [Ren'py][1]'s script language in order to describe the game.

This manual, partly based on Ren'py's own manual, describes how those scripts are made.

## A Simple Game

```python

	label start:
	
	    "Hi there! How was class?"
	
	    "Good..."
	
	    "I can't bring myself to admit that it all went in one ear and out the other."
	
	    "Are you going home now? Wanna walk back with me?"
	
	    "Sure!"
```

The script above is verty simple; it has no pictures, but describes a conversation between two characters.

The first line is a `label` statement. The label statement is used to give a name to a place in the program. In this case, we create a label named `start`. The start label is special, as it's where the script begin running when the user starts the game.

The other line are *say* statements; they declare that either the narrator or some character are saying something.

Note that all the say statements are indented by four spaces. This is because they are a block underneath the label statement. Just like in Ren'Py or in Python, blocks must be indented relative to the prior statement, and all of the statements in a block must be indented by the same amount.

## Characters

```python

	define s = Character('Sylvie', color="#c8ffc8")
	define m = Character('Me', color="#c8c8ff")
	
	label start:
	
	    s "Hi there! How was class?"
	    m "Good..."
	
	    "I can't bring myself to admit that it all went in one ear and out the other."
	
	    s "Are you going home now? Wanna walk back with me?"
	
	    m "Sure!"
	
```

The first and and second lines define characters. The first line defines a character with the short name of "s", the long name "Sylvie", with a name that is shown in a greenish color. (The colors are red-green-blue hex triples, as used in web pages.)

The second line creates a character with a short name "m", a long name "Me", with the name shown in a reddish color. Other characters can be defined by copying one of the character lines, and changing the short name, long name, and color.

We've also changed the say statements to use character objects instead of a character name string. This tells the program to use the characters we defined.

## Images

A visual novel isn't much of a visual novel without pictures. Here's another scene from "The Question". This also includes statements that show images to the player. This can fully replace the previous section of script, if you want to try it out.

```python

	define s = Character('Sylvie', color="#c8ffc8")
	define m = Character('Me', color="#c8c8ff")
	
	label start:
	
	    scene bg meadow
	
	    "After a short while, we reach the meadows just outside the neighborhood where we both live."
	
	    "It's a scenic view I've grown used to. Autumn is especially beautiful here."
	
	    "When we were children, we played in these meadows a lot, so they're full of memories."
	
	    m "Hey... Umm..."
	
	    show sylvie smile
	
	    "She turns to me and smiles. She looks so welcoming that I feel my nervousness melt away."
	
	    "I'll ask her...!"
	
	    m "Ummm... Will you..."
	
	    m "Will you be my artist for a visual novel?"
	
	    show sylvie surprised
	
	    "Silence."

```

all images from the screen, and displays a single image. This segment of script introduces two new statements. The scene statement on line 6 clears all images and displays a background image. The show statements on lines 16 and 26 display a sprite on top of the background, and change the displaying sprite, respectively.

In the script language, each image has a name. The name consists of a tag, and optionally one or more attributes. Both the tag and attributes should begin with a letter, and contain letters, numbers, and underscores. For example:


* In the scene statement on line 6, the tag is "bg", and the attribute is "meadow." By convention, background images should use the tag bg.
* In the first show statement on line 16, the tag is "sylvie", and the attribute is "smile".
* In the second show statement on line 26, the tag is "sylvie", and the attribute is "surprised".

VN32X looks for images in either the "img" or the "script" directory; the images must be in the "PNG" format.

For example, the following files, placed in either of the image directories, would define the following images:

    "bg meadow.jpg" -> meadow
    "sylvie_smile.png" -> sylvie smile
    "sylvie_surprised.png" -> sylvie surprised

## Menus, Labels, and Jumps

The menu statement lets presents a choice to the player:

```python

	    s "Sure, but what's a \"visual novel?\""
	
	menu:
	
	    "It's a videogame.":
	        jump game
	
	    "It's an interactive book.":
	        jump book
	
	label game:
	
	    m "It's a kind of videogame you can play on your computer or a console."
	
	    jump marry
	
	label book:
	
	    m "It's like an interactive book that you can read on a computer or a console."
	
	    jump marry
	
	label marry:
	
	    "And so, we become a visual novel creating duo."

```

This example shows how a menu can be used within the script. The menu statement introduces an in-game choice. It takes an indented block of lines, each consisting of a string followed by a colon. These are the menu choices that are presented to the player. Each menu choice takes its own indented block of lines, which is run when that menu choices is chosen.

In this example, each of the two menu choices runs a single jump statement. The jump statement transfers control to the a label defined using the label statement. After a jump, script statements following the label are run.

In the example above, after Sylvie asks her question, the player is presented with a menu containing two choices. If the player picked "It's a videogame.", the first jump statement is run, and the program will jump to the `game` label. This will cause the POV character to say "It's a story with pictures and music.", after which the game will jump to the `marry` label.

If there is no jump statement at the end of the block associated with the label, the program will continue on to the next statement. The last jump statement here is technically unnecessary, but is included since it makes the flow of the game clearer.

For now, labels must be defined in a file named *script.rpy* in the "script" directory. 




[1]: https://www.renpy.org/doc/html/quickstart.html
