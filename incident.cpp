MyIncident::MyIncident ()
{
    _IncidentNum = 0;
}

void MyIncident::AddIncident (size_t Vertex)
{
    if (Vertex >= _a.size())
        _a.resize(Vertex + 1, false);
    if (!_a[Vertex]) _IncidentNum++;
    _a[Vertex] = true;
}

void MyIncident::RemoveIncident (size_t Vertex)
{
    if (Vertex < _a.size())
    {
        if (_a[Vertex]) _IncidentNum--;
        _a[Vertex] = false;
    }
}

bool MyIncident::CheckIncident (size_t Vertex) const
{
    if (Vertex >= _a.size())
        return false;
    return _a[Vertex];
}

size_t MyIncident::IncidentNum () const
{
    return _IncidentNum;
}

size_t MyIncident::begin () const
{
    for (size_t i = 0; i < _a.size(); ++i)
        if (_a[i])
            return i;
    return End();
}

size_t MyIncident::end () const
{
    return _a.size();
}

~MyIncident::MyIncident (){}