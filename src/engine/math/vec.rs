use std::ops::{Add, Div, Mul, Neg, Sub};

trait Approximately {
    fn approximately(&self, other: &Self) -> bool;
}

impl Approximately for f64 {
    fn approximately(&self, other: &Self) -> bool {
        (self - other).abs() < 0.001
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Vec2<T>(pub T, pub T);

impl Vec2<f64> {
    pub fn intersect(p1: &Self, slope1: f64, p2: &Self, slope2: f64) -> Self {
        if p1.approximately(p2) {
            return p1.clone();
        }

        let localized_p2 = p2 - p1;

        let intersection_x = (localized_p2.1 - slope2 * localized_p2.0) / (slope1 - slope2);
        let intersection = Vec2(intersection_x, slope1 * intersection_x);

        intersection + p1
    }

    pub fn slope(&self) -> f64 {
        self.1 / self.0
    }

    pub fn perpendicular(&self) -> Self {
        Vec2(-self.1, self.0)
    }

    pub fn radius(&self) -> f64 {
        (self.0 * self.0 + self.1 * self.1).sqrt()
    }

    pub fn distance_squared(&self, other: &Self) -> f64 {
        let d_x = other.0 - self.0;
        let d_y = other.1 - self.1;

        d_x * d_x + d_y + d_y
    }

    pub fn distance(&self, other: &Self) -> f64 {
        self.distance_squared(other).sqrt()
    }
}

impl Approximately for Vec2<f64> {
    fn approximately(&self, other: &Self) -> bool {
        self.0.approximately(&other.0) && self.1.approximately(&other.1)
    }
}

impl<T: Neg<Output = T>> Neg for Vec2<T> {
    type Output = Vec2<T>;

    fn neg(self) -> Self::Output {
        Vec2(-self.0, -self.1)
    }
}

impl<T: Add<Output = T>> Add for Vec2<T> {
    type Output = Vec2<T>;

    fn add(self, rhs: Vec2<T>) -> Vec2<T> {
        Vec2(self.0 + rhs.0, self.1 + rhs.1)
    }
}

impl<T: Add<Output = T> + Copy> Add<&Vec2<T>> for Vec2<T> {
    type Output = Vec2<T>;

    fn add(self, rhs: &Vec2<T>) -> Vec2<T> {
        Vec2(self.0 + rhs.0, self.1 + rhs.1)
    }
}

impl<T: Sub<Output = T>> Sub for Vec2<T> {
    type Output = Vec2<T>;

    fn sub(self, rhs: Vec2<T>) -> Vec2<T> {
        Vec2(self.0 - rhs.0, self.1 - rhs.1)
    }
}

impl<T: Sub<Output = T> + Copy> Sub for &Vec2<T> {
    type Output = Vec2<T>;

    fn sub(self, rhs: &Vec2<T>) -> Vec2<T> {
        Vec2(self.0 - rhs.0, self.1 - rhs.1)
    }
}

impl<T: Mul<Output = T> + Copy> Mul<T> for Vec2<T> {
    type Output = Vec2<T>;

    fn mul(self, rhs: T) -> Vec2<T> {
        Vec2(self.0 * rhs, self.1 * rhs)
    }
}

impl<T: Div<Output = T> + Copy> Div<T> for Vec2<T> {
    type Output = Vec2<T>;

    fn div(self, rhs: T) -> Vec2<T> {
        Vec2(self.0 / rhs, self.1 / rhs)
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn vec2_neg() {
        let v = Vec2(5.0, -10.0);

        let actual = -v;
        assert_eq!(actual.0, -v.0);
        assert_eq!(actual.1, -v.1);
    }

    #[test]
    fn vec2_add() {
        let v1 = Vec2(1.0, -1.0);
        let v2 = Vec2(-5.0, 10.0);

        let actual = v1 + v2;
        assert_eq!(actual.0, v1.0 + v2.0);
        assert_eq!(actual.1, v1.1 + v2.1);

        let actual = v1 + &v2;
        assert_eq!(actual.0, v1.0 + v2.0);
        assert_eq!(actual.1, v1.1 + v2.1);
    }

    #[test]
    fn vec2_sub() {
        let v1 = Vec2(1.0, -1.0);
        let v2 = Vec2(-5.0, 10.0);

        let actual = v1 - v2;
        assert_eq!(actual.0, v1.0 - v2.0);
        assert_eq!(actual.1, v1.1 - v2.1);

        let actual = &v1 - &v2;
        assert_eq!(actual.0, v1.0 - v2.0);
        assert_eq!(actual.1, v1.1 - v2.1);
    }

    #[test]
    fn vec2_mul() {
        let v = Vec2(5.0, -10.0);
        let s = 5.0;

        let actual = v * s;
        assert_eq!(actual.0, v.0 * s);
        assert_eq!(actual.1, v.1 * s);
    }

    #[test]
    fn vec2_div() {
        let v = Vec2(5.0, -10.0);
        let s = 5.0;

        let actual = v / s;
        assert_eq!(actual.0, v.0 / s);
        assert_eq!(actual.1, v.1 / s);
    }

    #[test]
    fn vec2_slope() {
        let v = Vec2(5.0, -10.0);

        assert_eq!(v.slope(), v.1 / v.0);
    }

    #[test]
    fn vec2_perpendicular() {
        let v = Vec2(-5.0, 10.0);

        assert!(v.perpendicular() != v);
        assert_eq!(v.perpendicular().perpendicular().neg(), v);
    }

    #[test]
    fn vec2_radius() {
        assert!(Vec2(0.0, 0.0).radius().approximately(&0.0));
        assert!(Vec2(1.0, 0.0).radius().approximately(&1.0));
        assert!(Vec2(0.7071, -0.7071).radius().approximately(&1.0));
    }
}
