#pragma once

class Data {
    private:
        const void * m_data;

    public:
        Data();
        Data(const void * data);
        Data(const Data & src);
        Data & operator = (const Data & src);
        virtual ~Data();

        const void * get_data() const;
        void set_data(const void * data);
};
