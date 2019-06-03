/* global width height lambda maxSpeed coefCohes coefAlign coefSep */

const EPS = 1e-4;
var uid = 0;

class Boids { // eslint-disable-line no-unused-vars
	constructor (number, tailSize, influenceRadius, start, goal) {
		start = start || [Math.random() * width, Math.random() * height];
		goal = goal || [Math.random() * width, Math.random() * height];

		this.number = number;
		this.influenceRadius = influenceRadius;
		this.lambda = lambda;
		this.tailSize = tailSize;
		this.goal = goal;
		this.maxSpeed = maxSpeed;
		this.uid = uid++;

		this.coefCohes = coefCohes;
		this.coefSep = coefSep;
		this.coefAlign = coefAlign;

		this.boids = d3.range(number).map(() => new Boid(start, this));
	}

	updateSpeed (groups) {
		var centroid = this.getCentroid();
		this.boids.forEach(boid => boid.updateSpeed(centroid, groups));
	}

	updatePosition () {
		this.boids.forEach(boid => boid.updatePosition());
	}

	getCentroid() {
		return [
			d3.mean(this.boids, b => b.x),
			d3.mean(this.boids, b => b.y)
		];
	}
}


class Boid {
	constructor (start, group) {
		this.group = group;

		this.x = (2 * Math.random() - 1) * 125 + start[0];
		this.y = (2 * Math.random() - 1) * 125 + start[1];
		this.vx = 2 * Math.random() - 1,
		this.vy = 2 * Math.random() - 1,

		this.path = d3.range(this.group.tailSize).map(() => [this.x, this.y]);
		this.count = 0;
	}

	vectorTo ([x, y]) {
		return [x - this.x, y - this.y];
	}

	directionTo (vector) {
		var [vx, vy] = this.vectorTo(vector);
		var d = this.distanceTo(vector) + EPS;
		return [vx / d, vy / d];
	}

	distanceTo ([x, y]) {
		return norm([x - this.x, y - this.y]);
	}

	updateSpeed (centroid, groups) {
		var group = this.group,
			cohesionVector = this.getCohesionVector(centroid),
			alignementVector = this.getAlignementVector(),
			separationVector = this.getSeparationVector(groups);

		var newVx = group.coefCohes * cohesionVector[0] + group.coefAlign * alignementVector[0] + group.coefSep * separationVector[0],
			newVy = group.coefCohes * cohesionVector[1] + group.coefAlign * alignementVector[1] + group.coefSep * separationVector[1];

		[newVx, newVy] = renorm([newVx, newVy], group.maxSpeed);

		this.vx = group.lambda * this.vx + (1 - group.lambda) * newVx;
		this.vy = group.lambda * this.vy + (1 - group.lambda) * newVy;

		[this.vx, this.vy] = renorm([this.vx, this.vy], group.maxSpeed);
	}

	updatePosition () {
		this.x += this.vx;
		this.y += this.vy;

		var path = this.path,
			dx = this.vx,
			dy = this.vy,
			x = this.x,
			y = this.y,
			speed = this.group.maxSpeed,
			count = speed * 10,
			k1 = -5 - speed / 3;

		path[0][0] = this.x;
		path[0][1] = this.y;
		for (var j = 1; j < this.group.tailSize; j++) {
			var vx = x - path[j][0],
				vy = y - path[j][1],
				k2 = Math.sin(((this.count += count) + j * 3) / 300) / speed;
			path[j][0] = (x += dx / speed * k1) - dy * k2;
			path[j][1] = (y += dy / speed * k1) + dx * k2;
			speed = Math.sqrt((dx = vx) * dx + (dy = vy) * dy);
		}
	}

	getCohesionVector (vector) {
		var [vx, vy] = this.vectorTo(vector);
		[vx, vy] = [Math.log(Math.abs(vx) + 1) * Math.sign(vx), Math.log(Math.abs(vy) + 1) * Math.sign(vy)];
		return renorm([vx, vy]);
	}

	getCohesionVector2 (vector) {
		return this.directionTo(vector);
	}

	getAlignementVector () {
		return this.directionTo(this.group.goal);
	}

	getSeparationVector(groups) {
		var sum = [0, 0];

		groups.forEach(group => {
			group.boids.forEach(boid => {
				var d = this.distanceTo([boid.x, boid.y]) + EPS;
				if (EPS < d && d < this.group.influenceRadius) {
					// console.log(~~(this.x - boid.x), ~~d);
					// sum[0] += 1 / (this.x - boid.x) / d / d * 1000
					// sum[1] += 1 / (this.y - boid.y) / d / d * 1000
					sum[0] += (this.x - boid.x) / d / d * 10;
					sum[1] += (this.y - boid.y) / d / d * 10;
				}
			});
		});

		return (sum);
	}

	getSeparationVector2(groups) {
		var sum = [0, 0];

		groups.forEach(group => {
			group.boids.forEach(boid => {
				if (boid == this)
					return;
				var [vx, vy] = this.vectorTo([boid.x, boid.y]);
				sum[0] -= 1 / vx * 2;
				sum[1] -= 1 / vy * 2;
			});
		});

		return (sum);
	}

	getSeparationVector3() {
		var sum = [0, 0];

		this.group.boids.forEach((boid) => {
			var d = this.distanceTo([boid.x, boid.y]) + EPS;
			if (d < this.group.influenceRadius) {
				sum[0] += (this.x - boid.x) / d;
				sum[1] += (this.y - boid.y) / d;
			}
		});

		return renorm(sum);
	}

}


function norm([x, y]) {
	return Math.sqrt(x*x + y*y);
}


function renorm([x, y], newNorm) {
	newNorm = newNorm || 1;
	var oldNorm = norm([x, y]) + EPS;
	return [x *newNorm / oldNorm, y * newNorm / oldNorm];
}
