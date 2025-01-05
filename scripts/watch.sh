#!/usr/bin/env bash

while sleep 1; do
  ls *.{c,h} | entr -d -r -c bash -c 'echo -e "Watching for changes... (Hit ^C twice to exit)." && make all'
done
