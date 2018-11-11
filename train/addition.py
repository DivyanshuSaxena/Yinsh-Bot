def update_winner(player_id):
	print 'Updating winner for {0}'.format(player_id)
	if player_id == '1':
		print 'Player 1 is the winner'
		winner_filer = open('Yinsh-Bot/train/player1.dat', 'r')
		loser_filer = open('Yinsh-Bot/train/player2.dat', 'r')
	else:
		print 'Player 2 is the winner'
		winner_filer = open('Yinsh-Bot/train/player2.dat', 'r')
		loser_filer = open('Yinsh-Bot/train/player1.dat', 'r')

	contentw = winner_filer.readlines()
	contentl = loser_filer.readlines()
	contentw = [x.strip() for x in contentw] 
	contentl = [x.strip() for x in contentl] 
	print contentw
	print contentl
	
	last_resultw = contentw[0]
	last_resultl = contentl[0]
	num_winsw = int(contentw[1])
	num_winsl = int(contentl[1])
	weightsw = contentw[2]
	weightsl = contentl[2]
	print last_resultw
	print num_winsw

	if player_id == '1':
		winner_filew = open('Yinsh-Bot/train/player1.dat', 'w')
		loser_filew = open('Yinsh-Bot/train/player2.dat', 'w')
	else:
		winner_filew = open('Yinsh-Bot/train/player2.dat', 'w')
		loser_filew = open('Yinsh-Bot/train/player1.dat', 'w')

	winner_filew.write('1\n')
	winner_filew.write(str(num_winsw+1))
	winner_filew.write('\n')
	winner_filew.write(weightsw)

	loser_filew.write('0\n')
	loser_filew.write(str(num_winsl))
	loser_filew.write('\n')
	loser_filew.write(weightsl)
