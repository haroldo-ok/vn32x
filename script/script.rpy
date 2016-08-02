image bg lecturehall = "lecturehall.jpg"
image bg uni = "uni.jpg"

image sylvie normal = "sylvie_normal.png"

label start:
	scene bg lecturehall
	"Well, professor Eileen's lecture was interesting."
	
	show sylvie normal
	
	"Sylvie" "Oh, hi, do we walk home together?"
	m "Yes..."

	menu:
		"... to ask her right away.":
			jump rightaway
		"... to ask her later.":
			jump later
			
label rightaway:
	s "Oh, hi, do we walk home together?"
	m "Yes..."
	"I said and my voice was already shaking."

label later:
    "And so I decided to ask her later."
    "But I was indecisive."
    "I couldn't ask her that day, and I couldn't ask her later."
    "I guess I will never know now."
    ".:. Bad Ending."
