#!/bin/bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
SOURCE_DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

export LD_LIBRARY_PATH="$SOURCE_DIR/../lib:$LD_LIBRARY_PATH"
export QT_PLUGIN_PATH="$SOURCE_DIR/../lib:$QT_PLUGIN_PATH"

"$SOURCE_DIR/FastoRedis"
