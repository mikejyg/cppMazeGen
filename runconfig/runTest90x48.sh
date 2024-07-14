#!/usr/bin/env bash

log=test90x48.log
./run.sh 90 48 | tee $log
echo compare against golden
diff golden/$log $log
