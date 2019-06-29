BASEDIR=$(dirname $0)
echo $BASEDIR
echo $1
if [ $# -eq 0 ]; then
    echo "Please provide a trace file (gzip) as the argument"
else
    make -C ${BASEDIR}
    gzip -dc $1 | ${BASEDIR}/cache.exe ${BASEDIR}/params.cfg
fi