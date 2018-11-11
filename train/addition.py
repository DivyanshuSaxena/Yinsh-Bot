def update_winner(player_id):
	if player_id is 1:
		winner_filer = open('Yinsh-Bot/train/player1.dat', 'r')
	else:
		winner_filer = open('Yinsh-Bot/train/player2.dat', 'r')

	content = winner_filer.readlines()
	content = [x.strip() for x in content] 
	print content
	
	last_result = content[0]
	num_wins = int(content[1])
	weights = content[2]
	print last_result
	print num_wins

	if player_id is 1:
		winner_filew = open('Yinsh-Bot/train/player1.dat', 'w')
	else:
		winner_filew = open('Yinsh-Bot/train/player2.dat', 'w')

	winner_filew.write('1\n')
	winner_filew.write(str(num_wins+1))
	winner_filew.write('\n')
	winner_filew.write(weights)
