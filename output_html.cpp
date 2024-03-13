#include <iostream>
#include <sstream>
#include <vector>

/*
 * 1、复杂的构造过程交给构造器
 * 2、用户使用的过程中看不到构造器，但是调用的是构造器的方法
 * 3、这种代码设计使得原有的类变得简洁
 */
class HtmlBuilder;
class HtmlElement;

class HtmlElement
{
public:
    static HtmlBuilder *build(std::string x);

private:
    typedef HtmlElement Self;
    HtmlElement(std::string name, std::string text) : name(name), text(text) {}
    std::vector<HtmlElement> child;
    std::string name, text;
    friend std::ostream &operator<<(std::ostream &out, const Self &obj)
    {
        out << "<" << obj.name << ">";
        out << obj.text;
        for (const auto &o : obj.child)
        {
            out << o;
        }
        out << "</" << obj.name << ">";
        return out;
    }
    friend class HtmlBuilder;
};

class HtmlBuilder
{
public:
    typedef HtmlBuilder Self;

    HtmlBuilder(std::string root_name) : root(root_name, "") {}

    operator HtmlElement() const { return root; }

    Self &add_child(std::string name, std::string text)
    {
        HtmlElement("aa", "bb");
        root.child.push_back(HtmlElement(name, text));
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &out, const Self &obj)
    {
        out << obj.root;
        return out;
    }

private:
    HtmlElement root;
};

HtmlBuilder *HtmlElement::build(std::string root_name)
{
    return new HtmlBuilder(root_name);
}
int main()
{
    HtmlElement e = HtmlElement::build("ul")
                        ->add_child("li", "hello")
                        .add_child("li", "world");
    std::cout << e << std::endl;
    return 0;
}