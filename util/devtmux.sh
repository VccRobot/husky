tmux new-session -d
tmux split-window -v
tmux split-window -h
tmux new-window 'mutt'
tmux -2 attach-session -d
source setup.bash
