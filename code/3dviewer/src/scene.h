#ifndef SCENE_H
#define SCENE_H

#include <QtOpenGL/qgl.h>
#include <QString>
#include <list>

// CGAL
#include <CGAL/basic.h>
#include <CGAL/Simple_cartesian.h>
#include "polyhedron.h"
#include "tag.h"
#include "pointcloud.h"
#include "vectorfield.h"
#include <CGAL/IO/Polyhedron_iostream.h>

#include "types.h"

class Scene {
    public:
        typedef Vector_field::Vector Vector_3;
        typedef Point_cloud::Point Point_3;
        typedef Polyhedron::Base::Plane_3 Plane_3;

    private:
        // Polyhedral balls
        Polyhedron m_ball; // model
        std::vector<Vector_3> m_normalsBall;
        std::vector<Vector_ad> m_normalsBall_ad;
        std::vector<Polyhedron> m_balls; // translated balls

        // Point cloud
        Point_cloud m_pointcloud;
        std::vector<Point_ad> m_pointsAD;

        // Intersections
        typedef CGAL::Polyhedron_3<Kernel_ad, Items_tag<Kernel> > Polyhedron_tag;
        std::vector<Polyhedron_tag> m_inter;

        // Vector field
        Vector_field m_vectorfield;

        // rendering options
        bool m_smooth;
        bool m_view_ball;
        bool m_view_edges;
        bool m_view_facets;
        bool m_view_pointcloud;
        bool m_view_vectorfield;
        bool m_view_intersections;

        // parameters
        double m_radius;

    public:
        // life cycle
        Scene ();

        virtual ~Scene ();

        // file menu
        int open (QString filename);
        void save (QString filename);
        void saveNormals (QString filename);

        // algorithms menu
        bool ask_method (int& method);
        void compute_gradients (int method);
        void compute_balls ();
        void vector_field ();
        void nsteps ();
        void add_noise ();
        void random_ellipsoid ();
        void compute_intersections ();
        void setRadius (double radius) {
            m_radius = radius;
        }

        // toggle rendering options
        void toggle_view_smooth () { m_smooth = !m_smooth; }
        void toggle_view_ball () { m_view_ball = !m_view_ball; }
        void toggle_view_edges () { m_view_edges = !m_view_edges; }
        void toggle_view_facets () { m_view_facets = !m_view_facets; }
        void toggle_view_pointcloud () { m_view_pointcloud = !m_view_pointcloud; }
        void toggle_view_vectorfield () { m_view_vectorfield = !m_view_vectorfield; }
        void toggle_view_intersections () { m_view_intersections = !m_view_intersections; }

        // set visibility
        void set_visible_ball (bool visibility) { m_view_ball = visibility; }
        void set_visible_edges (bool visibility) { m_view_edges = visibility; }
        void set_visible_facets (bool visibility) { m_view_facets = visibility; }
        void set_visible_pointcloud (bool visibility) {
            m_view_pointcloud = visibility;
        }
        void set_visible_vectorfield (bool visibility) {
            m_view_vectorfield = visibility;
        }
        void set_visible_intersections (bool visibility) {
            m_view_intersections = visibility;
        }

        // clear scene options
        void clear_ball () {
            m_ball.clear();
            m_normalsBall.clear();
            m_normalsBall_ad.clear();
        }
        void clear_pointcloud () {
            m_pointcloud.clear();
            m_pointsAD.clear();
        }
        void clear_balls () { m_balls.clear(); }
        void clear_vectorfield () { m_vectorfield.clear(); }
        void clear_intersections () { m_inter.clear(); }

        // rendering
        void render ();
};

#endif // SCENE_H
