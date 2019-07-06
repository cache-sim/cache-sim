BASEDIR=$(dirname $0)

if [ $# -eq 0 ]; then
    echo "Usage:"
    echo "./run.sh -t|--trace <TRACE> -c|--config <CONFIG_FILE> [-i|--interactive] [-d|--debug]"
else
    ARGS=""
    TRACE=""
    CONFIG=""
    while [[ $# -gt 0 ]]; do
        case $1 in
            -i|--interactive)
                ARGS="$ARGS -DINTERACTIVE"
                shift
                ;;
            -d|--debug)
                ARGS="$ARGS -DDEBUG"
                shift
                ;;
            -t|--trace)
                TRACE="$2"
                shift
                shift
                ;;
            -c|--config)
                CONFIG="$2"
                shift
                shift
                ;;
            *)
                echo "Unidentified option: $1"
                rm parameters.cpp
                exit 1
                ;;
        esac
    done
    if [ $CONFIG == "" ]; then
        echo "Please provide a configuration"
    elif [ $TRACE == "" ]; then
        echo "Please provide a proper trace file"
    elif [ $(file --mime-type -b $TRACE) != "application/gzip" ]; then
        echo "Please provide a trace in gzip format"
    else
        ARGS="$ARGS$(cat params.cfg | awk '/[a-z]/ {print "-D" $1}' | uniq)"
        FILES=$(cat params.cfg | awk '/[a-z]/ {print "policies/" $1 ".cpp"}' | uniq)
        make -C ${BASEDIR} G++FLAGS="$ARGS" POLICY_FILES="$FILES"
        gzip -dc $TRACE | ${BASEDIR}/cache.exe $CONFIG
    fi 
fi