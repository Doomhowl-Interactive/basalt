#!/bin/bash
~/dev/notice/notice -i . -o ./TODO.md -v -e c,cpp && \
cat ./TODO.md
