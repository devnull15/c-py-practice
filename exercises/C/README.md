# C CODE WON'T BUILD?
If the build for any of the C projects fails because of CIUnit.h then you aren't in the schoolhouse docker container, use the following line:
`docker run -v '$(pwd):/code' -it registry.cybbh.space/17d/bolc/docker/build:latest /bin/bash`
then
`../../../scripts/build.sh`
from the project base directory (i.e. /exercises/C/4.1.6/)
