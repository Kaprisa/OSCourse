:loop
if exist %1\!nm! (
	set/a n+=1
	set nm=!n!_!f!
	goto loop
)