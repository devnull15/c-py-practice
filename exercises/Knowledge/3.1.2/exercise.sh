#!/bin/bash

QUIZ_REPO_FILE=https://git.cybbh.space/17d/bolc/public/python-quiz/-/raw/feb4935c5f952211ca8cc28f9ad340df16e3f273/quiz/__init__.py

if [ ! -e quiz.py ] ; then
	wget "${QUIZ_REPO_FILE}" -O quiz.py
fi

for quiz in `ls *.jqr 2>/dev/null`; do
       if [ ! -e ${quiz}.response.yaml ]; then 
		QUIZ_FILE=${quiz} python3 quiz.py
       fi
done
