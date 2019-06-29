if [ $# -eq 0 ]; then
    echo "Please provide a trace file (tar zip) as the argument"
else
    make
    gzip -dc $1 | ./cache.exe
fi