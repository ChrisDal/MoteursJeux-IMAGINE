#include "Rotoform.h"

// ================================================
// Constructors 
// ================================================

Rotoform::Rotoform()
    : m_rotx(glm::mat3x3(1.0f)), m_roty(glm::mat3x3(1.0f)),
    m_rotz(glm::mat3x3(1.0f)), m_rotation(glm::mat3x3(1.0f))
{
    // All Rotations are set to identity 
    
    // global transform 
    m_rotation = glm::mat3x3(1.0f); 
}


Rotoform::Rotoform(const glm::mat3x3& rotx, const glm::mat3x3& roty, const glm::mat3x3& rotz)
    : m_rotx(rotx), m_roty(roty), m_rotz(rotz)
{
    // set main rotation
    processTransformation();
}


// Constructor: with angles (alpha, axe x), (beta, axe y ), (gamma, axe z) 
Rotoform::Rotoform(float alpha, float beta, float gamma, bool inRad = false)
    : m_rotx(glm::mat3x3()), m_roty(glm::mat3x3()), m_rotz(glm::mat3x3())
{
    if (!inRad)
    {
        setRotationX(alpha);
        setRotationY(beta);
        setRotationZ(gamma);
    }
    else
    {
        setRotationX_rad(alpha);
        setRotationY_rad(beta);
        setRotationZ_rad(gamma);
    }

    // set m_rotation
    processTransformation();

}



// ================================================
// Rule of 5 
// ================================================

// Copy Constructor
Rotoform::Rotoform(const Rotoform& source)
{
    m_rotx = source.m_rotx;
    m_roty = source.m_roty;
    m_rotz = source.m_rotz;
    m_rotation = m_rotz * m_roty * m_rotx;

}

// Copy Assignement 
Rotoform& Rotoform::operator=(const Rotoform& source)
{

    if (this == &source)
        return *this;

    m_rotx = source.m_rotx;
    m_roty = source.m_roty;
    m_rotz = source.m_rotz;
    m_rotation = m_rotz * m_roty * m_rotx;


    return *this;
}



// Move Constructor
Rotoform::Rotoform(Rotoform&& source)
{
    m_rotx = source.m_rotx;
    m_roty = source.m_roty;
    m_rotz = source.m_rotz;
    m_rotation = m_rotz * m_roty * m_rotx;

    source.resetRotation();

}

// Destructor 
Rotoform::~Rotoform() {};



// ================================================
// Protected Functions 
// ================================================

// Update main rotation Matrix 
inline void Rotoform::processTransformation()
{
    m_rotation = m_roty * m_rotx * m_rotz;
}

// convert degrees to rad 
inline float Rotoform::degreeToRad(const float& theta) { 
    return static_cast<float>(theta * glm::pi<float>() / 180.0f); 
}


// Rotation around X from an angle in eta
inline glm::mat3x3 Rotoform::getMat3rotationX_rad(float eta)
{
    return glm::mat3x3(1.0, 0.0, 0.0,
                0.0, std::cos(eta), -std::sin(eta),
                0.0, std::sin(eta), std::cos(eta));

}

// Rotation around Y from an angle in eta
inline glm::mat3x3 Rotoform::getMat3rotationY_rad(float eta)
{
    return glm::mat3x3( std::cos(eta), 0.0, std::sin(eta),
                       0.0, 1.0, 0.0,
                       -std::sin(eta), 0.0, std::cos(eta) );

}

// Rotation around Z from an angle in radians
inline glm::mat3x3 Rotoform::getMat3rotationZ_rad(float eta)
{
    return glm::mat3x3(std::cos(eta), -std::sin(eta), 0.0,
                       std::sin(eta), std::cos(eta), 0.0,
                       0.0, 0.0, 1.0 );
    
}

// Rotation around X from an angle in degrees
inline glm::mat3x3 Rotoform::getMat3rotationX(float angle)
{
    float thetaRad = degreeToRad(angle);
    return glm::mat3x3(1.0, 0.0, 0.0,
                        0.0, std::cos(thetaRad), -std::sin(thetaRad),
                        0.0, std::sin(thetaRad), std::cos(thetaRad));
}

// Rotation around Y from an angle in degrees
inline glm::mat3x3 Rotoform::getMat3rotationY(float angle)
{
    float thetaRad = degreeToRad(angle);
    return glm::mat3x3(std::cos(thetaRad), 0.0, std::sin(thetaRad),
                       0.0, 1.0, 0.0,
                       -std::sin(thetaRad), 0.0, std::cos(thetaRad));

}

// Rotation around Z from an angle in degrees
inline glm::mat3x3 Rotoform::getMat3rotationZ(float angle)
{
    float thetaRad = degreeToRad(angle);
    return glm::mat3x3(std::cos(thetaRad), -std::sin(thetaRad), 0.0,
                       std::sin(thetaRad), std::cos(thetaRad), 0.0,
                       0.0, 0.0, 1.0 );
}




// ================================================
// SET Rotation 
// ================================================

// Set rotation theta in degrees around axe X 
inline void Rotoform::setRotationX(float theta) {
    m_rotx = getMat3rotationX(theta);
    // update Rotation matrix
    processTransformation();
}

// Set rotation theta in degrees around axe Y
inline void Rotoform::setRotationY(float theta) {
    m_roty = getMat3rotationY(theta);
    // update Rotation matrix
    processTransformation();
}


// Set rotation theta in degrees around axe Z 
inline void Rotoform::setRotationZ(float theta) {
    m_rotz = getMat3rotationZ(theta);
    // update Rotation matrix
    processTransformation();
}


// Set rotation eta in radians around axe X 
inline void Rotoform::setRotationX_rad(float eta) {
    m_rotx = getMat3rotationX_rad(eta);
    // update Rotation matrix
    processTransformation();
}

// Set rotation eta in radians around axe Y 
inline void Rotoform::setRotationY_rad(float eta) {
    m_roty = getMat3rotationY_rad(eta);
    // update Rotation matrix
    processTransformation();
}

// Set rotation eta in radians around axe Z
inline void Rotoform::setRotationZ_rad(float eta) {
    m_rotz = getMat3rotationZ_rad(eta);
    // update Rotation matrix
    processTransformation();
}


// ================================================
// Public Rotation 
// ================================================

// Set Global rotation = set value of rotation
void Rotoform::setRotation(float alpha, float beta, float gamma)
{
    setRotationX(alpha);
    setRotationY(beta);
    setRotationZ(gamma);
    // update Rotation matrix
    processTransformation();

}

// Set Global rotation in radians = set value of rotation
void Rotoform::setRotation_rad(float alpha, float beta, float gamma)
{
    setRotationX_rad(alpha);
    setRotationY_rad(beta);
    setRotationZ_rad(gamma);

    // update Rotation matrix
    processTransformation();

}

// ----------------------------------------------------
// Add Rotation on axis according to theta in degrees  
// ----------------------------------------------------

void Rotoform::addRotationX(float theta)
{
    m_rotx = getMat3rotationX(theta) * m_rotx;
    // update Rotation matrix
    processTransformation();
}


void Rotoform::addRotationY(float theta)
{
    m_roty = getMat3rotationY(theta) * m_roty;
    // update Rotation matrix
    processTransformation();
}

void Rotoform::addRotationZ(float theta)
{
    m_rotz = getMat3rotationZ(theta) * m_rotz;
    // update Rotation matrix
    processTransformation();
}


// ----------------------------------------------------
// Add Rotation on axis according to eta in radians  
// ----------------------------------------------------
void Rotoform::addRotationX_rad(float eta)
{
    m_rotx = getMat3rotationX_rad(eta) * m_rotx;
}


void Rotoform::addRotationY_rad(float eta)
{
    m_roty = getMat3rotationY_rad(eta) * m_roty;
}

void Rotoform::addRotationZ_rad(float eta)
{
    m_rotz = getMat3rotationZ_rad(eta) * m_rotz;
}


// Add Global rotation = rotate previous matrix , in degrees
void Rotoform::addRotation(float alpha, float beta, float gamma)
{
    if (alpha != 0.0)
        addRotationX(alpha);

    if (beta != 0.0)
        addRotationY(beta);

    if (gamma != 0.0)
        addRotationZ(gamma);

    // update Rotation matrix
    processTransformation();

}

// Add Global rotation = rotate previous matrix , in radians
void Rotoform::addRotation_rad(float alpha, float beta, float gamma)
{
    if (alpha != 0.0)
        addRotationX_rad(alpha);

    if (beta != 0.0)
        addRotationY_rad(beta);

    if (gamma != 0.0)
        addRotationZ_rad(gamma);

    // update matrix
    processTransformation();

}


// ================================================
// Global Reset
// ================================================


// Reset all 4 rotation matrix 
void Rotoform::resetRotation()
{
    
    m_rotx = glm::mat3x3(1.0f); 
    m_roty = glm::mat3x3(1.0f); 
    m_rotz = glm::mat3x3(1.0f);
    m_rotation = glm::mat3x3(1.0f);

}

// ================================================
// Getters / setters
// ================================================


glm::mat3x3 Rotoform::getRotation() const { return m_rotation; }


inline void Rotoform::setMatrixRotation(const glm::mat3x3& mat)
{
    m_rotation = mat;
}


// ================================================
// Apply To
// ================================================

// Apply Rotation to Point, returns a glm::vec3
/*glm::vec3 Rotoform::applyToPoint(const glm::vec3& point)
{
    processTransformation();

    glm::vec3 rotpoint = glm::vec3(point);
    rotpoint[0] = rotpoint[0] * m_rotation(0,0) + rotpoint[1] * m_rotation(0,1) + rotpoint[2] * m_rotation(0,2);
    rotpoint[1] = rotpoint[0] * m_rotation(1,0) + rotpoint[1] * m_rotation(1,1) + rotpoint[2] * m_rotation(1,2);
    rotpoint[2] = rotpoint[0] * m_rotation(2,0) + rotpoint[1] * m_rotation(2,1) + rotpoint[2] * m_rotation(2,2);


    return rotpoint;

}*/

// Apply Rotation to Point, returns a glm::vec3
glm::vec3 Rotoform::applyToPoint(const glm::vec3& point, const glm::vec3& rotcenter)
{
    processTransformation();

    glm::vec3 rotpoint = point - rotcenter;
    rotpoint = rotpoint * m_rotation; 

    /*rotpoint[0] = rotpoint[0] * m_rotation[0, 0] + rotpoint[1] * m_rotation[0, 1] + rotpoint[2] * m_rotation[0, 2];
    rotpoint[1] = rotpoint[0] * m_rotation[1, 0] + rotpoint[1] * m_rotation[1, 1] + rotpoint[2] * m_rotation[1, 2];
    rotpoint[2] = rotpoint[0] * m_rotation[2, 0] + rotpoint[1] * m_rotation[2, 1] + rotpoint[2] * m_rotation[2, 2];*/

    rotpoint += rotcenter;

    return rotpoint;

}

// Apply Rotation to Vectors, returns a glm::vec3
glm::vec3 Rotoform::applyToVector(const glm::vec3& vec)
{
    processTransformation();

    glm::vec3 rotpoint = glm::vec3(vec);
    /*rotpoint[0] = rotpoint[0] * m_rotation(0, 0) + rotpoint[1] * m_rotation(0, 1) + rotpoint[2] * m_rotation(0, 2);
    rotpoint[1] = rotpoint[0] * m_rotation(1, 0) + rotpoint[1] * m_rotation(1, 1) + rotpoint[2] * m_rotation(1, 2);
    rotpoint[2] = rotpoint[0] * m_rotation(2, 0) + rotpoint[1] * m_rotation(2, 1) + rotpoint[2] * m_rotation(2, 2);*/

    return rotpoint * m_rotation; ;
}

// Apply Rotation to Versors, returns a glm::vec3
glm::vec3 Rotoform::applyToVersor(const glm::vec3& versor)
{
    processTransformation();

    glm::vec3 rotpoint = glm::vec3(versor);

    /*rotpoint[0] = rotpoint[0] * m_rotation(0, 0) + rotpoint[0] * m_rotation(1, 0) + rotpoint[0] * m_rotation(2, 0);
    rotpoint[1] = rotpoint[1] * m_rotation(0, 1) + rotpoint[1] * m_rotation(1, 1) + rotpoint[1] * m_rotation(2, 1);
    rotpoint[2] = rotpoint[2] * m_rotation(0, 2) + rotpoint[2] * m_rotation(1, 2) + rotpoint[2] * m_rotation(2, 2);*/

    return rotpoint * m_rotation;
}

// -----------------
// OTHERS
// -----------------

std::ostream& operator<<(std::ostream& stream, const Rotoform& rotation)
{
    stream << "Rotation : (";
    for (unsigned int i = 0; i < 9; i++)
    {
        stream << rotation.m_rotation(int(i / 3), i % 3) << ",";
        if ((i + 1) % 3 == 0 && i > 0)
        {
            stream << ")\n         : (";
        }

    }
    return stream;
}

inline glm::mat3x3 Rotoform::getInvMatRot()
{
    this->processTransformation();

    return glm::transpose(m_roty) * glm::transpose(m_rotx) * glm::transpose(m_rotz);
}

// set transposed matrix for all rotation 
Rotoform Rotoform::getInvRot()
{
    Rotoform roto;
    roto.m_rotx = glm::transpose(m_rotx);
    roto.m_roty = glm::transpose(m_roty);
    roto.m_rotz = glm::transpose(m_rotz);
    roto.processTransformation();

    return roto;

}



// ================================================
// Operators overloading
// ================================================

// On ajoute les rotations
Rotoform Rotoform::operator+(const Rotoform& other)
{
    Rotoform roto;
    roto.m_rotx = this->m_rotx * other.m_rotx;
    roto.m_roty = this->m_roty * other.m_roty;
    roto.m_rotz = this->m_rotz * other.m_rotz;
    roto.processTransformation();

    return roto;
}


