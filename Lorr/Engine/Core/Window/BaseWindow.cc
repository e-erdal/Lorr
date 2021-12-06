#include "BaseWindow.hh"

namespace Lorr
{
    const SystemMetrics::Display *BaseWindow::GetDisplay(u32 monitor) const
    {
        ZoneScoped;
        
        if (monitor >= m_SystemMetrics.Displays.size()) return nullptr;

        return &m_SystemMetrics.Displays[monitor];
    }

}  // namespace Lorr