# JQR 3.1.17

```
title: 'Describe terms associated with compiling, linking, debugging and executables:'
condition: B
standard: A
sub_items:
  3.1.17.a: difference between PE and ELF
  3.1.17.b: portable executable (PE)
  3.1.17.c: executable and linkable format (ELF)
  3.1.17.d: difference between a library (shared object /dll) and a regular executable
    program
  3.1.17.e: calling convention /application binary interface (ABI)

```

## Instructions

### Complete the exercise

To complete these exercises, upload a JQR item (3.x.x.x.jqr) to the web quiz here: https://17d.cybbh.io/bolc/public/web-quiz/

When you complete an item, download and save the `*.response.yaml` file in this same folder. Then commit the change to GitLab.

### Commit the exercise

```
git add *response.yaml
git commit
git push
```

## CI Pipeline

Note: if your CI pipeline does not run when you commit, please ensure you have made a merge request to merge
your branch into the primary branch. This will trigger the rules for the CI Job configured for this item.


## References

1. https://blog.kowalczyk.info/articles/pefileformat.html
2. https://stackoverflow.com/questions/9688200/difference-between-shared-objects-so-static-libraries-a-and-dlls-so
