use crate::engine::math::polygon::Polygon;
use crate::engine::math::vec::Vec2;

struct Segment<'a>(pub &'a Vec2<f64>, pub &'a Vec2<f64>);

impl<'a> Segment<'a> {
    pub fn intersects(_other: &Segment<'a>) -> bool {
        todo!()
    }
}

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
        _vertices: &Vec<Vec2<f64>>,
        _ear_vertices: &Vec<ReferenceVertex>,
        _base_index: usize,
        _diagonal_index: usize,
    ) -> bool {
        // let base_vertex = &vertices[base_index];
        // let diagonal_vertex = &vertices[diagonal_index];

        // let mut first_vertex = ear_vertices[base_index].next;
        // while ear_vertices[first_vertex].next != base_index {
        //     let next_vertex = ear_vertices[first_vertex].next;

        //     if first_vertex == diagonal_index || next_vertex == diagonal_index {
        //         continue;
        //     }

        //     todo!();

        //     first_vertex = ear_vertices[first_vertex].next;
        // }

        todo!();
    }
}
