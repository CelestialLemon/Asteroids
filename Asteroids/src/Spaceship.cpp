#include "Spaceship.h"

// initialize the spaceship params
Spaceship::Spaceship() 
: m_thrust(600000), m_rigidbody(Rigidbody(4000, 300)), gunMaxCharge(100), gunRechargeSpeed(500), gunCurrentCharge(0.0f), collider(4)
{
    m_bulletTexture.loadFromFile("./res/images/bullet.png");

    collider.setPoint(0, sf::Vector2f(27, 0));
    collider.setPoint(1, sf::Vector2f(-27, 27));
    collider.setPoint(2, sf::Vector2f(-17, 0));
    collider.setPoint(3, sf::Vector2f(-27, -27));
}

// update the position of the ship by calculating next position given currentPos and velocity and dt
void Spaceship::updatePosition(float dt) {
    
    sf::Vector2f currentVelocity = m_rigidbody.GetVelocity();

    float dy = currentVelocity.y * dt;
    float dx = currentVelocity.x * dt;

    Move(sf::Vector2f(dx, dy));


    // warp check
    auto currentPosition = getPosition();

    // 720.0f is the height and width of screen in pixels
    currentPosition.x = currentPosition.x < 0.0f ? 720.0f : currentPosition.x;
    currentPosition.x = currentPosition.x > 720.0f ? 0.0f : currentPosition.x;

    currentPosition.y = currentPosition.y < 0.0f ? 720.0f : currentPosition.y;
    currentPosition.y = currentPosition.y > 720.0f ? 0.0f : currentPosition.y;


    setPosition(currentPosition);
}

void Spaceship::updateRotation(float dt, sf::Vector2f mousePosition) {
    
    auto playerPosition = getPosition();
    // set player position to center of sprite
    //playerPosition.x += 32.0f;
    //playerPosition.y += 32.0f;

    // vector pointing from center of player to mouse
    auto mouseDirection = mousePosition - playerPosition;

    // normalize the direction
    float magnitude = sqrtf((mouseDirection.x * mouseDirection.x) + (mouseDirection.y * mouseDirection.y));
    
    mouseDirection.x /= magnitude;
    mouseDirection.y /= magnitude;
    //float magnitude = sqrtf((mouseDirection.x * mouseDirection.x) + (mouseDirection.y * mouseDirection.y));
    //mouseDirection /= magnitude;

    // find angle using x-cord
    float angle = acosf(mouseDirection.x);
    // convert angle to deg
    angle *= (180.0f / 3.14159f);

    // check quadrant
    if(mouseDirection.y < 0.0f) angle = 360.0f - angle;

    setRotation(angle);
}

void Spaceship::Fire() {

    // if gun reaches max charge fire
    if(gunCurrentCharge >= (float)gunMaxCharge) {

        // get position of spaceship
        sf::Vector2f fireFrom = getPosition();

        // distance from center of spaceship to its front tip
        float distanceFromCenter = 20.0f;

        float dy = distanceFromCenter * sinf(getRotation() * 3.14159f / 180.0f); 
        float dx = distanceFromCenter * cosf(getRotation() * 3.14159f / 180.0f); 

        // add some displacement in the forward direction so bullet is fired from tip of plane
        fireFrom += sf::Vector2f(dx, dy);

        // create new bullet
        Bullet* b = new Bullet(fireFrom, sf::Vector2f(dx, dy), m_bulletTexture);

        // add new bullet to the map of bullets
        m_bullets.insert(b);

        // reset gun charge
        gunCurrentCharge = 0;
    }
}

float DegToRad(float deg) {
    return deg * 3.14159 / 180.0f;
}

sf::Vector2f TransformPoint(sf::Vector2f point, float rotation, sf::Vector2f translation) {
    
    sf::Vector2f transformedPoint(
        point.x * cos(DegToRad(rotation)) - point.y * sin(DegToRad(rotation)),
        point.y * cos(DegToRad(rotation)) + point.x * sin(DegToRad(rotation))
    );

    transformedPoint += translation;
    return transformedPoint;
}

// take input to move the ship
// TODO : normalize the movement vector so as to apply uniform thurst
void Spaceship::update(float dt, sf::RenderWindow& window) {
    
    // position
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        m_rigidbody.AddForce(sf::Vector2f(-m_thrust * dt, 0));
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_rigidbody.AddForce(sf::Vector2f(m_thrust * dt, 0));
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        m_rigidbody.AddForce(sf::Vector2f(0, -m_thrust * dt));
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_rigidbody.AddForce(sf::Vector2f(0, m_thrust * dt));

    updatePosition(dt);

    // rotation
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    updateRotation(dt, (sf::Vector2f)mousePosition);

    // fire code
    
    // increase the charge of gun every frame depending on rechargeSpeed
    gunCurrentCharge += (float)gunRechargeSpeed * dt;

    // if already at maxCharge dont increase
    gunCurrentCharge = gunCurrentCharge > gunMaxCharge ? gunMaxCharge : gunCurrentCharge;

    // if pressed button, fire
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
        Fire();

    // update positions of bullets and draw them to screen
    
    // also check which bullets went out bound and have to be deleted
    // save their addresses in toDelete array, we will delete them after updating all, else it will mess up the foreach loop

    std::vector<Bullet*> toDelete;
    
    for(auto bullet : m_bullets) {
        bullet->update(dt, window);
        bullet->draw(window);

        if(!bullet->isInBounds()) toDelete.push_back(bullet);
    }

    // remove the outofbound bullets from map and delete them
    for(auto bullet : toDelete) {
        m_bullets.erase(bullet);
        delete(bullet);
    }
}


void Spaceship::AsteroidBulletCollision(const std::unordered_set<Asteroid*>& asteroids) {

    // container to store reference to bullets that have hit an asteroid
    std::vector<Bullet*> bullets_hit;

    for(auto bullet : m_bullets) {
        for(auto asteroid : asteroids) {
            // check if the bullet is inside asteroid
            bool hasBulletHit = asteroid->IsPointInside(bullet->getPosition());

            if(hasBulletHit) {
                // if hit, add to container
                bullets_hit.push_back(bullet);

                // reduce asteroid hitpoints
                asteroid->Hit(40);
                break;
            }
        }
    }

    // delete bullets that have hit an asteroid
    for(auto bullet : bullets_hit) {
        m_bullets.erase(bullet);
        delete(bullet);
    }
}

struct Triangle {
    sf::Vector2f a, b, c;

    Triangle(sf::Vector2f _a, sf::Vector2f _b, sf::Vector2f _c)
    : a(_a), b(_b), c(_c) {}
};

sf::Vector2f CalculateNormal(sf::Vector2f pointA, sf::Vector2f pointB) {
    // get vector pointing from one point to the other
    sf::Vector2f directionVector = pointB - pointA;
    // swap co-efficents and negate one to calculate normal (not normalized)
    return sf::Vector2f(directionVector.y, -directionVector.x);
}

float Vector2fDot (sf::Vector2f a, sf::Vector2f b) {
    return (a.x * b.x + a.y * b.y);
}

bool TriangleSATCollision(const std::vector<sf::Vector2f>& vertices1, const std::vector<sf::Vector2f>& vertices2, sf::RenderWindow& window) {
    // vertices 1 and 2 will have 3 vertices each as they are triangles
    // there will be 6 normals
    
    std::vector<sf::Vector2f> normals(vertices1.size() + vertices2.size());

    
    // sf::VertexArray t1(sf::Triangles, 3);
    // t1[0] = vertices1[0]; t1[0].color = sf::Color::Red;
    // t1[1] = vertices1[1]; t1[1].color = sf::Color::Red;
    // t1[2] = vertices1[2]; t1[2].color = sf::Color::Red;
    // window.draw(t1);

    normals[0] = CalculateNormal(vertices1[0], vertices1[1]);
    normals[1] = CalculateNormal(vertices1[1], vertices1[2]);
    normals[2] = CalculateNormal(vertices1[2], vertices1[0]);

    normals[3] = CalculateNormal(vertices2[0], vertices2[1]);
    normals[4] = CalculateNormal(vertices2[1], vertices2[2]);
    normals[5] = CalculateNormal(vertices2[2], vertices2[0]);

    for(auto normal : normals) {
        
        float t1Min = FLT_MAX, t1Max = FLT_MIN;

        for(auto vertex : vertices1) {
            float projection = Vector2fDot(vertex, normal);
            t1Min = std::min(t1Min, projection);
            t1Max = std::max(t1Max, projection);
        }

        float t2Min = FLT_MAX, t2Max = FLT_MIN;

        for(auto vertex : vertices2) {
            float projection = Vector2fDot(vertex, normal);
            t2Min = std::min(t2Min, projection);
            t2Max = std::max(t2Max, projection);
        }

        if(t1Max < t2Min || t2Max < t1Min) {
            return false;
        }
    }

    sf::ConvexShape t1(3), t2(3);
    t1.setFillColor(sf::Color::Red);
    t2.setFillColor(sf::Color::Red);
    t1.setPoint(0, vertices1[0]);
    t1.setPoint(1, vertices1[1]);
    t1.setPoint(2, vertices1[2]);
    t2.setPoint(0, vertices2[0]);
    t2.setPoint(1, vertices2[1]);
    t2.setPoint(2, vertices2[2]);
    window.draw(t1);
    window.draw(t2);
    return true;
}

bool Spaceship::AsteroidSpaceshipCollision(const std::unordered_set<Asteroid*>& asteroids, sf::RenderWindow& window) {
    std::vector<Triangle> spaceshipColliderTriangles = {
        Triangle(
            TransformPoint(collider.getPoint(0), getRotation(), getPosition()),
            TransformPoint(collider.getPoint(1), getRotation(), getPosition()),
            TransformPoint(collider.getPoint(2), getRotation(), getPosition())
        ),
        Triangle(
            TransformPoint(collider.getPoint(0), getRotation(), getPosition()),
            TransformPoint(collider.getPoint(2), getRotation(), getPosition()),
            TransformPoint(collider.getPoint(3), getRotation(), getPosition())
        )
    };

    for(auto asteroid : asteroids) {
        std::vector<Triangle> asteroidColliderTriangles;

        const sf::ConvexShape asteroidShape = asteroid->GetConvexShape();
        const size_t nPoints = asteroidShape.getPointCount();

        for(size_t i = 0; i < nPoints - 1; i++) {
            asteroidColliderTriangles.push_back(
                Triangle(
                    TransformPoint(sf::Vector2f(0, 0), asteroidShape.getRotation(), asteroidShape.getPosition()),
                    TransformPoint(asteroidShape.getPoint(i), asteroidShape.getRotation(), asteroidShape.getPosition()),
                    TransformPoint(asteroidShape.getPoint(i + 1), asteroidShape.getRotation(), asteroidShape.getPosition())
                )
            );
        }

        asteroidColliderTriangles.push_back(
            Triangle(
                TransformPoint(sf::Vector2f(0, 0), asteroidShape.getRotation(), asteroidShape.getPosition()),
                TransformPoint(asteroidShape.getPoint(nPoints - 1), asteroidShape.getRotation(), asteroidShape.getPosition()),
                TransformPoint(asteroidShape.getPoint(0), asteroidShape.getRotation(), asteroidShape.getPosition())
            )
        );

        for(auto asteroidTriangle : asteroidColliderTriangles) {
            for(auto spaceshipTriangle : spaceshipColliderTriangles) {
                std::vector<sf::Vector2f> vertices1 = {asteroidTriangle.a , asteroidTriangle.b, asteroidTriangle.c};
                std::vector<sf::Vector2f> vertices2 = {spaceshipTriangle.a , spaceshipTriangle.b, spaceshipTriangle.c};

                if(TriangleSATCollision(vertices1, vertices2, window)) {
                    return true;
                }
            }
        }
    }
    return false;
}
