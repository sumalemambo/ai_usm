// Represents an entity of the assignment problem
class Entity {
    public:
        int id;
        float size;

        // Class constructor
        Entity(int id, float size) {
            this->id = id;
            this->size = size;
        }
};