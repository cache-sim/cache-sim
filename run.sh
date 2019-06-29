if [ $# -eq 0 ]; then
    echo "Please provide a trace file (gzip) as the argument"
else
    make
    gzip -dc $1 | ./cache.exe
fi