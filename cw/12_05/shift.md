===shift===

1, 2, 3

shift 

$1: 2
$2: 3

changes the $# too -> decrements by one

```bash
for i in {1..$#}; do
	echo $1
	shift
done
```

this works

