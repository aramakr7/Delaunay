#include "common.h"
#include "Triangulation.h"
#include "QuadEdge.h"

INITIALIZE_EASYLOGGINGPP


std::vector<Vertex *> makePoints(int n)
{
    // Generate a field of random vertices for debug/demonstration

    srand(time(NULL));

    std::vector<std::vector<int>> buffer;

    // Build a buffer list
    for (int i = 0; i < n; i++)
    {
        std::vector<int> xy = {rand() % 512, rand() % 512};
        buffer.push_back(xy);
    }

    // Sort it lexicographically; we need this step
    //std::sort(buffer.begin(), buffer.end());
    //buffer.erase(std::unique(buffer.begin(), buffer.end()), buffer.end());

    std::vector<Vertex *> points;

    // Turn it into Verts for the convenience of our algorithm
    for (size_t i = 0; i < buffer.size(); i++)
    {
        points.push_back(new Vertex(buffer[i][0], buffer[i][1]));
    }

    points.clear();
    points.push_back(new Vertex(88, 101));
    points.push_back(new Vertex(444, 208));
    points.push_back(new Vertex(137, 362));
    points.push_back(new Vertex(478, 405));
    points.push_back(new Vertex(456, 220));

    return points;
}


void InitializeLogger()
{

    el::Configurations defaultConf;
    defaultConf.setToDefault();
    // Values are always std::string
    defaultConf.set(el::Level::Info,
                    el::ConfigurationType::Format, "%datetime %level %loc %msg");
    // default logger uses default configurations
    el::Loggers::reconfigureLogger("default", defaultConf);
}

int main()
{
    InitializeLogger();
    std::vector<Vertex *> points = makePoints(8);
    std::vector<Vertex *> unprocessedPoints;
    std::vector<QuadEdge *> edgeList;
    Edge *startingEdge;

    QuadEdge *q = new QuadEdge();
    q->edges[0].setOrigin(new Vertex(0,0));
    q->edges[0].setDest(new Vertex(10,10));

    LOG(INFO) << *q;

   // exit(1);
    Triangulation tri(points[0], points[1], points[2]);
    LOG(INFO) << "made triangle";

    for (size_t i = 3; i < points.size(); i++)
    {
        std::cout << "\n\n\n\n\n\n\n\n\n" << std::endl;
        LOG(INFO) << "point: " << i + 1;
        LOG(INFO) << points[i]->x << ", " << points[i]->y;
        tri.insertSite(points[i]);
    }

    return 0;
}
