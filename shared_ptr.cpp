#include <iostream>

class A
{
public:
    int x, y;
    friend std::ostream &operator<<(std::ostream &out, const A &obj)
    {
        out << obj.x << ' ' << obj.y << '\n';
        return out;
    }
    A(int x = 0, int y = 0) : x(x), y(y)
    {
        std::cout << "constructor" << std::endl;
    }
    ~A() { std::cout << "destructor" << std::endl; }
};

namespace my
{
    class ptr_date
    {
    public:
        ptr_date(A *ptr, int *cnt = nullptr) : ptr(ptr), cnt(cnt)
        {
            if (this->cnt == nullptr)
                this->cnt = new int(1);
        };
        void increase_one() const { *cnt += 1; }
        void decrease_one() const
        {
            *cnt -= 1;
            if (*cnt == 0)
                delete ptr;
        }
        bool operator==(const ptr_date &p) const
        {
            return p.ptr == ptr && p.cnt == cnt;
        }
        ~ptr_date()
        {
            decrease_one();
            if (*cnt == 0)
                delete cnt;
        }
        A *ptr;
        int *cnt;
    };
    class shared_ptr
    {
    public:
        shared_ptr(A *ptr) : p(ptr) {}

        shared_ptr(const shared_ptr &ths) : p(ths.p) { p.increase_one(); }
        A *operator->() const { return p.ptr; }

        A &operator*() const { return *p.ptr; }

        shared_ptr &operator=(const shared_ptr &obj)
        {
            if (obj.p == p)
                return *this;
            p.decrease_one();
            p = obj.p;
            p.increase_one();
            return *this;
        }
        int use_count() { return *p.cnt; }
        ~shared_ptr() = default;

    private:
        ptr_date p;
    };
    shared_ptr make_shared() { return shared_ptr(new A()); }
}

int main()
{
    {
        my::shared_ptr p = my::make_shared();
        p->x = 3, p->y = 4;
        std::cout << *p << std::endl;
        std::cout << "p.use_count = " << p.use_count() << std::endl;
        my::shared_ptr q = p;
        std::cout << "p.use_count = " << p.use_count() << std::endl;
        std::cout << "q.use_count = " << q.use_count() << std::endl;
        p = nullptr;
        std::cout << "q.use_count = " << q.use_count() << std::endl;
        q = nullptr;
        std::cout << "end main" << std::endl;
    }
    //    std::shared_ptr<A> p = std::make_shared<A>();
    //    std::cout << "p.use_count = " << p.use_count() << std::endl;
    //    std::shared_ptr<A> q = p;
    //    std::cout << "p.use_count = " << p.use_count() << std::endl;
    //    std::cout << "q.use_count = " << q.use_count() << std::endl;
    //    p = nullptr;
    //    std::cout << "q.use_count = " << q.use_count() << std::endl;
    //    q = nullptr;
    //    std::cout << "end main" << std::endl;
    return 0;
}
