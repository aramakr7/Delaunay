
#ifdef matplotlib
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;
#endif

#include "common.h"
#include "Triangulation.h"
#include "QuadEdge.h"
#include <tuple>
#include <vector>


INITIALIZE_EASYLOGGINGPP

typedef std::tuple<std::vector<Edge*>, std::vector<Edge*> > EdgeTuple;

std::vector<Vertex *> makePoints(int n)
{
    // Generate a field of random vertices for debug/demonstration

    srand(time(NULL));

    std::vector<std::vector<int>> buffer;

    // Build a buffer list
    for (int i = 0; i < n; i++)
    {
        std::vector<int> xy = {rand() % 50, rand() % 50};
        buffer.push_back(xy);
    }

    // Sort it lexicographically; we need this step
    std::sort(buffer.begin(), buffer.end());
    buffer.erase(std::unique(buffer.begin(), buffer.end()), buffer.end());

    std::vector<Vertex *> points;

    points.push_back(new Vertex(41, 23));
    points.push_back(new Vertex(43, 16));
    points.push_back(new Vertex(29, 39));
    points.push_back(new Vertex(3, 32));
    points.push_back(new Vertex(3, 42));

    // Turn it into Verts for the convenience of our algorithm
    // for (size_t i = 0; i < buffer.size(); i++)
    // {
    //     points.push_back(new Vertex(buffer[i][0], buffer[i][1]));
    // }

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
    std::vector<Vertex *> points = makePoints(5);
    EdgeTuple edges;


   // exit(1);

   // Insertion
    Triangulation tri(points[0], points[1], points[2]);
    LOG(INFO) << "made triangle";

    for (size_t i = 3; i < points.size(); i++)
    {
        std::cout << "\n\n\n\n\n\n\n\n\n" << std::endl;
        LOG(INFO) << "point: " << i + 1;
        LOG(INFO) << points[i]->x << ", " << points[i]->y;
        tri.insertSite(points[i]);
    }


    for (size_t i = 0; i < tri.m_edges.size(); i++)
    {
        std::cout << "" << tri.m_edges[i] << std::endl;
    }
    std::cout << "\n\n";


    // Divide and Conquer
    // Triangulation tri{};
    // edges = tri.divideAndConquer(points);

    // for (size_t i = 0; i < tri.m_edges.size(); i++)
    // {
    //     std::cout << "\n" << tri.m_edges[i] << std::endl;
    // }
    // std::cout << "\n\n";

    
    // Preparing data for plotting
#ifdef matplotlib
    std::vector<double> x, y;
    for (Edge* edge : tri.m_edges) {
        Vertex* origin = edge->getOrigin();
        Vertex* dest = edge->getDest();

        // Append origin and destination for each edge
        x.push_back(origin->x);
        y.push_back(origin->y);
        x.push_back(dest->x);
        y.push_back(dest->y);

        // Optional: Separate lines if you don't want a continuous line
        x.push_back(NAN); // NaN breaks the line in matplotlib plots
        y.push_back(NAN);
    }

    // Plotting
    plt::plot(x, y, "r-"); // 'r-' for red lines
    plt::title("Edge Plot");
    plt::xlabel("X axis");
    plt::ylabel("Y axis");
    plt::show();
#endif

    return 0;
}
