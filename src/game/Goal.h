#ifndef HEADER_GOAL_H
#define HEADER_GOAL_H

class Cube;

/**
 * Goal for model.
 */
class Goal {
    private:
        enum eSATISFY {
            SATISFY_TRUE,
            SATISFY_FALSE,
            SATISFY_IGNORE
        };

        eSATISFY m_alive;
        eSATISFY m_out;
    private:
        Goal(eSATISFY out, eSATISFY alive);
    public:
        static Goal noGoal();
        static Goal outGoal();
        static Goal escapeGoal();

        bool isSatisfy(const Cube *model) const;
};

#endif
