#ifndef HEADER_MINMAX_H
#define HEADER_MINMAX_H

/**
 * Minimum.
 */
inline int
min(int one, int second)
{
    return one <= second ? one : second;
}
/**
 * Maximum.
 */
inline int
max(int one, int second)
{
    return one >= second ? one : second;
}

#endif


