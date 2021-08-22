use crate::engine::math::polygon::Polygon;

struct ReferenceVertex {
    prev: usize,
    next: usize,
    is_ear: bool,
}

pub struct ReferenceVertices<'a> {
    polygon: &'a Polygon,
    ear_vertices: Vec<ReferenceVertex>,
}

impl<'a> ReferenceVertices<'a> {
    pub fn new(polygon: &'a Polygon) {
        let vertices = polygon.vertices();
        let mut ear_vertices = Vec::new();
        ear_vertices.reserve(vertices.len());

        let last_vertex_index = vertices.len() - 1;

        ear_vertices.push(ReferenceVertex {
            prev: last_vertex_index,
            next: 1,
            is_ear: false,
        });

        for i in 1..last_vertex_index {
            ear_vertices.push(ReferenceVertex {
                prev: i - 1,
                next: i + 1,
                is_ear: false,
            });
        }

        ear_vertices.push(ReferenceVertex {
            prev: last_vertex_index - 1,
            next: 0,
            is_ear: false,
        })
    }

    fn is_diagonal(
        vertices: &Vec<Vec2<f64>>,
        ear_vertices: &Vec<ReferenceVertex>,
        base_index: usize,
        diagonal_index: usize,
    ) -> bool {
        let base_vertex = vertices.
    }
}
