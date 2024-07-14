#!/usr/bin/env bash

{ time ./run.sh -b ; } 2>&1 | tee test.log

echo compare against golden
diff golden/test.log test.log
