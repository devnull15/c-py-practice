# JQR Repository

## About

This repository is your student Basic Job Qualification Record for the Cyberspace Capabilities Developer work role. 

You will find each JQR Item in an issue for this project.

Each Issue has its own dedicated branch (identified in the issue), along with an accompanying merge request. 

As you complete JQR items, commit your code in the appropriate branch. The pipeline will run tests to assist in identifying problems in your code.
Once you are ready for a code review, let your instructor know. They will conduct the code review and provide feedback. Once an instructor
has blessed off on the submission, a certified senior developer will approve the merge request, effectively signing off that submission. 

## Cheating Notice

These items must be individual efforts without assistance from others. Students may use public resources and personal notes, but may not copy code from others or share solutions. References and source material should be cited in-line with a note on the source and how it was used. 

Students caught sharing code or otherwise cheating in the JQR may be punished under UCMJ. Additionally, failure to complete the JQR as an individual effort without assistance from others is considered failure to complete the JQR and may result in the student being dropped from the course and ineligible for the developer work-role. 

### Giving Help

If you would like to help another trainee on their JQR in an item you have finished, you may provide the following forms of assistance:

* Share publicly-accessible reference material
 * "Hey I found a great reference for that knowledge item here at this website."
* Share troubleshooting techniques
 * "Here is a flag you can add with valgrind that shows line numbers for your crash traces"
* Share tips for the JQR repository, git, GitLab, or anything related to the mechanics of how to use the project.
 * "Here is how I clone and switch branches. Here is how I go to view the latest CI pipeline." 

In instances where someone is stuck troubleshooting code, you may not view or provide code to assist troubleshooting. You *can* assist with understanding stack traces or errors. For example, you could look at a trace of a crash and provide feedback, so long as you do not see the code that caused the crash. 

However, during the Basic Skill Level Exam itself, none of these are allowed. No communication with other students or trainees related to the exam is permissible. 

## Getting Started

This project is designed to facilitate individual completion of every JQR item by you in coordination with your mentor/instructor and a senior developer. 

Every JQR item has been populated in an associated issue. Your project comes pre-built with corresponding branches for these 
issues, of the format `<issue_number>-<jqr_item>`.

When you want to work on a JQR item, 
[create a merge request](https://docs.gitlab.com/ee/user/project/merge_requests/creating_merge_requests.html) from
your branch to the primary branch ("master"). This will activate conditional rules in the [pipeline configuration](./.gitlab-ci.yml) to
run the test cases on your submission.

Mark your merge request as a work-in-progress ("WIP: my work for JQR item 3.x.x") until you are ready for review. Please also mention the issue number in the text of the merge request for easy reference.


### GitLab CI 
There is a GitLab CI pipeline for every JQR item. In order for a job to run for a given JQR item, a commit should be made in the corresponding
branch. These pipelines are a tool to assist the trainee in testing their code, but final verification of code completed is 
conducted via manual code review by a senior developer. 

You can review the latest pipeline for an item in its merge request. Additionally, you can see all pipelines in the pipelines view within GitLab. 

### Directory Structure

Your primary place to work is in the `exercises` folder. The file structure is as follows:

```
exercises/
├── C
│   └── 3.X.X
├── Knowledge
│   └── 3.X.X
├── Python
│   └── 3.X.X
└── Special
    └── 3.X.X
```

You will need to switch to the branch for a JQR item to find the exercise source for that item. 

C and Python items can be run and tested locally. See specific build instructions
under Compiling and Running.  

Knowledge items are completed locally and support partial auto-grading in the web-quiz tool. They are graded for record via GitLab CI pipeline. 

Special items are designed per-item and have individual README files. 

### GitLab Pages

There is a compiled summary of your JQR repository located at:

https://17d.cybbh.io/bolc/students/jqr/active/${STUDENT_USERNAME}

Alternatively, you can find your project URL under Settings -> Pages

![Web-based JQR Rollup](https://git.cybbh.space/17d/bolc/public/public-files/-/raw/master/jqr_web_screenshot.png)

As you complete individual items and they merge into your primary branch,
your overall roll-up will be updated. 

When you complete the JQR, you can save PDF version of your jqr from
your Pages project at the project URL under the filename: jqr.pdf

### Completed Pipeline

When all items are merged into the primary branch and completed, your JQR
pipeline should look like this:

![Completed JQR Pipeline](https://git.cybbh.space/17d/bolc/public/public-files/-/raw/master/jqr_completed.gif)


## Compiling and Running

## Local Build Environment

To run your JQR items locally, you will need to configure a build
environment that is consistent with the one used in the GitLab CI
pipeline.

You can find the build environment used for test cases in the following Dockerfile: 

https://git.cybbh.space/17d/bolc/docker/build/-/blob/master/Dockerfile

Recommend usage is to either use this docker environment to test your code or to run the same commands to configure your own Linux build environment on an Ubuntu 20.04 environment.

### Using Docker

The easiest method is to use the docker image. To use the docker image, you can run your code in docker using the following command:

```
docker run -v "$(pwd):/code" -it registry.cybbh.space/17d/bolc/docker/build:latest /bin/bash
cd /code
ls -al
```

Note that `$(pwd)` should be run from your code directory or should be hard-coded to the full path of your source code.

### Local Python Testing

To run your test cases locally, navigate to the folder for your specific JQR item and run the following:

```
python3 tests/__init__.py
```

### Local C Testing

To test your C code locally, navigate to the folder for your specific JQR item and run the following:

```
bash ../../../scripts/build.sh
```

Alternatively, you can run the cmake build commands directly with:

```
cmake -H. -Bbuild
cmake --build build
```

After your build is complete, run the executable

```
./build/x.x.x_exe
```

## Knowledge Items

Knowledge items come in the format of Multiple Choice, True/False, Multiple Answer, and Short Answer questions. 

Each JQR sub-item has its own YAML file that defines the questions for that item. You can then use one of
two methods to generate a "<item_filename>.response.yaml" file, which you should add back to your
repository to submit. 

### Web-based quiz tool

The *recommended* way of completing the quiz is to use the [web-based Quiz Tool](https://17d.cybbh.io/bolc/public/web-quiz/)
to generate a response yaml for submission. [Source code stored here.](https://git.cybbh.space/17d/bolc/public/web-quiz/)

While you still must submit your solution for credit, this tool provides instant feedback on whether an 
item is right or wrong (except for regex-based solutions, in which case auto-grade is configured with a single
correct answer that meets the regex rather than all possible answers).

Everything in this tool is client-based, meaning no files leave your computer after you load the page. 

### Alternative, CLI-based approach

NOTE: this approach has been deprecated by the web-based tool mentioned previously

Every Knowledge item has a "exercise.sh" script. When you run this exercise script it will pull down a 
Python CLI-based graphical tool that will read the JQR items, present you with questions 
in the GUI, and store your answers in a new file. 

Note that the quiz tool is a work-in-progress, but is mostly functional. 


### Gitlab CI pipeline 

Once your response file is generated, you can add and commit that file to the project and it will
grade your responses within the CI pipeline job. You can view those responses in the CI job output
 for that item. 

You may then edit your response file to update your answers and resubmit. You may resubmit your 
responses as many times as necessary to get the correct answer. Once all items are correct, your 
mentor or instructor will verify your answers and merge your solutions into your primary branch.

## Test Cases that capture stdout or stderr

In some instances, the functions being tested are expected to print to stdout.
In a subset of those exercises, the test cases use custom file descriptors 
and require you to use them in your test code when you write to stdout. This
allows the test case to capture your intended output for the test case,
but also print to normal stdout for your own debugging statements without
impacting the test cases. 

In some of these exercises, this is referenced in the README for that exercise.
However, not all exercises have been updated to add this. If you get
compile errors referencing "g_arrays_fd_(stdout|stderr|stdin)", you will
want to include these lines in your solution:  

```
#include<stdarg.h>
volatile int g_arrays_fd_stdout = STDOUT_FILENO;
volatile int g_arrays_fd_stderr = STDERR_FILENO;
volatile int g_arrays_fd_stdin = STDIN_FILENO;

/**
 * @brief provides a wrapper for printf to control where stdout goes
 * 
 * @param format the format string to pass to *printf
 * @param ... the additional variables to pass to *printf
 */
static void local_printf(const char *format, ...)
{
    va_list args; // thanks to https://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html
    va_start(args, format);
    vdprintf(g_arrays_fd_stdout, format, args);
    va_end(args);
}


```

## Rebasing after patches

At times, there may be a need to push out a bugfix or patch to your JQR. 
Those patches will be pushed to the primary branch ("master"), and if safe, 
to JQR branches you have not yet started. However, if are already working a branch 
and a patch is pushed for content related to your branch, you will need to rebase 
your JQR branch from master. 

To rebase, from your JQR branch (XXX-3.X.X), run these commands:

```
git pull origin master && git rebase origin/master
```

Rebasing pulls the changes at the base of your commit log, and then replays all of your commits on top of it. You can learn more about rebasing here: https://git-scm.com/book/en/v2/Git-Branching-Rebasing, https://stackoverflow.com/questions/36148602/git-pull-vs-git-rebase)

After rebasing, you will need to force push your rebase back to origin (GitLab). You can do this with:

```
git push --force-with-lease
```

This is necessary because upstream has different hashes for the commits than the local rebase version. Before you do this, you will want to verify that your rebase did not delete any of your files. The force-push will overwrite upstream commits. 


NOTE: if you come across errors in rebasing, you generally should be able to safely prefer the master branch patches. We won't push patches that modify student submission files. 

This means you can run rebase like this: 

```
git rebase -X ours origin/master
```

In this case, this sets the strategy option that is passed to `git merge` when rebase merges. Normally, "ours" in this strategy prefers the current branch. But in the rebase, the roles are reverse so "ours" refers to "origin/master" in the command given.

The verbose work flow for rebase is:
```
git checkout master         # patches are pushed to master
git pull                    # grab the latest patch
git checkout XXX-3.x.x      # switch back to your branch you ar working
git rebase origin/master    # replay your commits on top of the patch
#if you have rebase conflicts, you can abort and rerun 
#the following way to tell git to prioritize the patch's version when
#there is a conflict
#git rebase -X ours origin/master 
git push --force-with-lease +XXX-3.x.x # finally, push changes back to GitLab
```

## Submitting an Item

When you have completed an item, go to the merge request for that item and make a comment saying you are ready for review. To make things easier for your reviewer, copy the text of the JQR item into the merge request so they can remember everything in the JQR for that item.

Then go to the Teams channel for your class and provide a link to the Merge Request in a comment. For trainees outside of class,
reach out to your mentor to let them know when your item is ready for review. 
