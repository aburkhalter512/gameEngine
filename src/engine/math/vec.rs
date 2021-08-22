use std::ops::{Add, Div, Mul, Neg, Sub};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Vec2<T>(pub T, pub T);

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

impl<T: Sub<Output = T>> Sub for Vec2<T> {
    type Output = Vec2<T>;

    fn sub(self, rhs: Vec2<T>) -> Vec2<T> {
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
    }

    #[test]
    fn vec2_sub() {
        let v1 = Vec2(1.0, -1.0);
        let v2 = Vec2(-5.0, 10.0);

        let actual = v1 - v2;
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
}
