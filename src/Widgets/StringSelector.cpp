#include <Widgets/StringSelector.hpp>

StringSelector::StringSelector()
{

}

 StringSelector::StringSelector(QWidget* parent, std::vector<std::string> list) : QComboBox(parent)
 {
     for (std::string item : list)
     {
         addItem(item);
     }
 }
