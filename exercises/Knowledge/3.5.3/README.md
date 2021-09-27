# JQR 3.5.3

```
title: 'describe the terms and concepts associated with concurrency:'
condition: B
standard: A
sub_items:
  3.5.3.a: threading
  3.5.3.b: locks
  3.5.3.c: race conditions
  3.5.3.d: deadlocks
  3.5.3.e: scheduling modules (ie, round-robin, etc)
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

1. _Operating Systems: Internals and Design Principles_. Pearson Education, 2014.
2. https://en.cppreference.com/w/cpp/thread/shared_mutex
3. https://www.geeksforgeeks.org/difference-between-shared-lock-and-exclusive-lock/
4. https://www.tutorialspoint.com/lightweight-process-lwp
5. https://www.cs.uct.ac.za/mit_notes/database/htmls/chp13.html#locking-techniques-for-concurrency-control - 3.5.3.d.jqr - concurrency control problem
