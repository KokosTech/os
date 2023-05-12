# one

find . -printf '%n\n' | sort -n | tail -n 5

# two

stat -c '%h %n' */* | sort -n | tail -n 5