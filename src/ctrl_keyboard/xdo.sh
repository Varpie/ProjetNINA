WID=`xdotool search "Mozilla Firefox" | head -1`
xdotool windowactivate $WID
xdotool key ctrl+l
xdotool key a
