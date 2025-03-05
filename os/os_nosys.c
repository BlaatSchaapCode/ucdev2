#include <reent.h>

[[gnu::weak]] int _close_r (struct _reent *, int){ return -1; }
[[gnu::weak]] int _fstat_r (struct _reent *, int, struct stat *){ return -1; }
[[gnu::weak]] int _getpid_r (struct _reent *){ return -1; }
[[gnu::weak]] int _isatty_r (struct _reent *, int){ return -1; }
[[gnu::weak]] int _kill_r (struct _reent *, int, int){ return -1; }
[[gnu::weak]] _off_t _lseek_r (struct _reent *, int, _off_t, int){ return -1; }
[[gnu::weak]] _ssize_t _read_r (struct _reent *, int, void *, size_t){ return -1; }
[[gnu::weak]] _ssize_t _write_r (struct _reent *, int, const void *, size_t){ return -1; }




