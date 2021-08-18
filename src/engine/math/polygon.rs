use crate::engine::math::vec::Vec2;

#[derive(Debug, PartialEq, Eq)]
pub enum Error {
    InsufficientVertices,
}

#[derive(Debug)]
pub struct Polygon(Vec<Vec2<f64>>);

impl Polygon {
    pub fn new(vertices: Vec<Vec2<f64>>) -> Result<Self, Error> {
        if vertices.len() < 3 {
            Err(Error::InsufficientVertices)
        } else {
            Ok(Self(vertices))
        }
    }

    pub fn vertices(&self) -> &Vec<Vec2<f64>> {
        &self.0
    }

    pub fn translate(mut self, translation: Vec2<f64>) -> Self {
        for vertex in &mut self.0 {
            *vertex = *vertex + translation;
        }

        self
    }

    pub fn is_clockwise(&self) -> bool {
        let prev_vertices = self.0.iter().cycle().skip(self.0.len() - 1);
        let vertex_pairs = prev_vertices.zip(self.0.iter());

        let area = vertex_pairs.fold(0f64, |area, (prev, cur)| {
            area + ((cur.0 - prev.0) * (cur.1 + prev.1))
        });

        area > 0.0
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn polygon_create() {
        let expected_vertices = vec![Vec2(0.0, 0.0), Vec2(0.0, 0.0), Vec2(0.0, 0.0)];
        let polygon = Polygon::new(expected_vertices.clone()).unwrap();

        assert_eq!(polygon.vertices(), &expected_vertices)
    }

    #[test]
    fn polygon_create_fails_insufficient_vertices() {
        let err = Polygon::new(vec![Vec2(0.0, 0.0), Vec2(0.0, 0.0)]).unwrap_err();
        assert_eq!(err, Error::InsufficientVertices);
    }

    #[test]
    fn polygon_translate_empty() {
        let expected_vertices = vec![Vec2(1.0, 1.0), Vec2(2.0, 1.0), Vec2(1.0, 2.0)];
        let polygon = Polygon::new(expected_vertices.clone())
            .unwrap()
            .translate(Vec2(0.0, 0.0));

        assert_eq!(polygon.vertices(), &expected_vertices)
    }

    #[test]
    fn polygon_translate() {
        let vertices = vec![Vec2(1.0, 1.0), Vec2(2.0, 1.0), Vec2(1.0, 2.0)];
        let expected_translation = Vec2(1.0, -1.0);
        let polygon = Polygon::new(vertices.clone())
            .unwrap()
            .translate(expected_translation);

        let expected_vertices: Vec<_> = vertices
            .into_iter()
            .map(|vertex| vertex + expected_translation)
            .collect();
        assert_eq!(polygon.vertices(), &expected_vertices)
    }

    #[test]
    fn polygon_is_clockwise_false() {
        let vertices = vec![Vec2(1.0, 1.0), Vec2(2.0, 1.0), Vec2(1.0, 2.0)];
        let polygon = Polygon::new(vertices.clone()).unwrap();

        assert_eq!(polygon.is_clockwise(), false);
    }

    #[test]
    fn polygon_is_clockwise_true() {
        let vertices = vec![Vec2(1.0, 2.0), Vec2(2.0, 1.0), Vec2(1.0, 1.0)];
        let polygon = Polygon::new(vertices.clone()).unwrap();

        assert_eq!(polygon.is_clockwise(), true);
    }
}
