#ifndef HEADER_COUNTADVISOR_H
#define HEADER_COUNTADVISOR_H

/**
 * Interface to advise countdown.
 */
class CountAdvisor {
    public:
        virtual ~CountAdvisor() {}
        virtual int getCountForSolved() const = 0;
        virtual int getCountForWrong() const = 0;
};

#endif
