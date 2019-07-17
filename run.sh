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
                ARGS="$ARGS -DINTERACTIVE -lcurses "
                shift
                if [[ "$ARGS" =~ "-DDEBUG" ]]; then
                    echo "-i and -d options are not supported together"
                    exit 1
                fi
                ;;
            -d|--debug)
                ARGS="$ARGS -DDEBUG"
                shift
                if [[ "$ARGS" =~ "-DINTERACTIVE" ]]; then
                    echo "-i and -d options are not supported together"
                    exit 1
                fi
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
        make -C ${BASEDIR} G++FLAGS="$ARGS"
        gzip -dc $TRACE | ${BASEDIR}/cache.exe $CONFIG
    fi 
fi