#include "cbpp/mesh.h"

namespace cbpp {
    const Array<Vertex>& Mesh::GetArray() const {
        return m_aVerts;
    }

    size_t Mesh::Size() const {
        return m_aVerts.Length();
    }

    bool Mesh::Resize(size_t iNewSize) {
        return m_aVerts.Resize(iNewSize);
    }

    bool Mesh::IsValid() const {
        return m_aVerts.IsValid() && (m_aVerts.Length() >= 3);
    }

    Vertex& Mesh::At(size_t iIndex) {
        iIndex = iIndex % m_aVerts.Length();
        return m_aVerts.At(iIndex);
    }

    Vertex& Mesh::operator[](size_t iIndex) {
        return this->At(iIndex);
    }

    void Mesh::PushVertex(const Vertex& vOther) {
        m_aVerts.PushBack(vOther);
        this->Sort();
    }

    void Mesh::ComputeMid() {
        m_vMidPos = Vec2(0);
        size_t iLength = m_aVerts.Length();
        for(size_t i = 0; i < iLength; i++) {
            m_vMidPos = m_vMidPos + m_aVerts.At(i).XY();
        }
        m_vMidPos = m_vMidPos / iLength;
    }

    void Mesh::Sort() {
        
    }
}
