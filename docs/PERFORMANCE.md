# Performance Notes

## Current Behavior

- Storage is file-based JSON with full load/parse on each service operation.
- Transaction log appends are O(1) and write-only.

## Expected Scale

This project is intended for small-scale, single-process CLI usage. It is not optimized for high concurrency or large datasets.

## Profiling Tips

- Build with `-DCMAKE_BUILD_TYPE=RelWithDebInfo` and use `perf` or `Instruments`.
- Look for hot spots in JSON parse/serialize and account lookup.

## Potential Improvements

- Add in-memory caching with explicit reload on write.
- Replace linear account lookup with indexed map.
- Move storage to a database backend for larger datasets.
