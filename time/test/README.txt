TestTimeLib may fail if console output is particularly slow. I have found this
to be a problem under Windows. Redirecting the output to a file should solve
this problem. (File I/O is faster because it is cached.)

