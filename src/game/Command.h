#ifndef HEADER_COMMAND_H
#define HEADER_COMMAND_H


/**
 * Command interface.
 */
class Command {
    public:
        virtual ~Command() {}
        /**
         * Return true when command has finished his work.
         * @param count number of calls
         * @return true for finish
         */
        virtual bool finish(int count) = 0;
};

#endif
