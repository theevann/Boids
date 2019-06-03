/* global width:true height:true Boids tailSize currentGroup influenceRadius nBoid showInfluence timeUpdateGoal createSimulation */

function createSimulation(nGroups) { // eslint-disable-line no-unused-vars

	var container = document.getElementById('container');
	width = container.clientWidth;
	height = container.clientHeight;

	d3.select('#container').selectAll('svg').remove();
	var svg = d3.select('#container').append('svg')
		.attr('width', width)
		.attr('height', height)
		.on('click', function () { simulation.group(currentGroup).goal = d3.mouse(this); });

	var headGroup, influenceCircles, tailGroup, goalGroup;

	var simulation = {
		groups: d3.range(nGroups).map(() => new Boids(nBoid, tailSize, influenceRadius)),
		showInfluence: showInfluence
	};

	simulation.start = function () {
		[headGroup, influenceCircles, tailGroup, goalGroup] = drawGroups(svg, this.groups);
		this.timer = d3.timer(updateSimulation);
		// this.timer = d3.interval(updateSimulation, 100);
	};

	simulation.stop = function () {
		this.timer && this.timer.stop();
	};

	simulation.restart = function () {
		simulation.stop();
		simulation.start();
	};

	simulation.group = function (uid) {
		return this.groups.find(g => g.uid == uid);
	};

	simulation.removeGroup = function (uid) {
		var index = this.groups.findIndex(g => g.uid == uid);
		if (index == -1) return;
		this.groups.splice(index, 1);
		simulation.restart();
	};

	simulation.addGroup = function () {
		this.groups.push(new Boids(nBoid, tailSize, influenceRadius));
		simulation.restart();
	};

	simulation.nextGroup = function (uid) {
		var index = this.groups.findIndex(g => g.uid == uid);
		if ((index == -1) || (index + 1 == this.groups.length)) return null;
		return this.groups[index + 1];
	};

	simulation.previousGroup = function (uid) {
		var index = this.groups.findIndex(g => g.uid == uid);
		if (index - 1 < 0) return null;
		return this.groups[index - 1];
	};

	function updateSimulation () {
		var update = updateGoal();

		simulation.groups.forEach(function (group) {
			group.updateSpeed(simulation.groups);
			group.updatePosition();
			if (update) group.goal = [Math.random() * width, Math.random() * height];
		});


		headGroup.attr('transform', headTransform);
		tailGroup.attr('d', tailPath);

		goalGroup
			.attr('cx', d => d.goal[0])
			.attr('cy', d => d.goal[1]);

		influenceCircles
			.attr('cx', d => d.x)
			.attr('cy', d => d.y)
			.attr('opacity', () => simulation.showInfluence ? .5 : 0)
			.attr('r', d => d.group.influenceRadius);
	}

	return simulation;
}

var updateGoal = (function () {
	var lastUpdate = 0;
	return function () {
		var time = (new Date()).getTime();
		if (timeUpdateGoal != 0 && time - lastUpdate > timeUpdateGoal) {
			lastUpdate = time;
			return true;
		}
		return false;
	};
})();

function headTransform(d) {
	return 'translate(' + [d.x, d.y] + ')rotate(' + (Math.atan2(d.vy, d.vx) * 180 / Math.PI) + ')';
}

function tailPath(d) {
	return 'M' + d.join('L');
}

function drawGroups (svg, groups) {
	var gr = svg.selectAll('g.group')
		.data(groups, g => g.uid);

	gr.exit().remove();

	gr = gr.enter().append('g')
		.classed('group', true);

	var g = gr.selectAll('g')
		.data(b => b.boids)
		.enter().append('g')
		.classed('boid', true);

	g.append('ellipse')
		.classed('head', true)
		.attr('rx', 6.5)
		.attr('ry', 4);

	g.append('path')
		.datum(function(d) { return d.path.slice(0, 3); })
		.classed('mid', true);

	g.append('path')
		.datum(function(d) { return d.path; })
		.classed('tail', true);

	g.append('circle')
		.classed('influence', true)
		.attr('cx', (d) => d.x)
		.attr('cy', (d) => d.y)
		.attr('r', (d) => d.group.influenceRadius)
		.attr('fill', 'none')
		.attr('opacity', '0.5')
		.attr('stroke', 'yellow')
		.attr('stroke-width', '2px');

	gr.append('circle')
		.classed('goal', true)
		.attr('cx', d => d.goal[0])
		.attr('cy', d => d.goal[1])
		.attr('r', 6)
		.attr('fill', 'red');

	var influenceCircles = d3.selectAll('.influence');
	var headGroup = d3.selectAll('.head');
	var tailGroup = d3.selectAll('.tail,.mid');
	var goalGroup = d3.selectAll('.goal');

	return [headGroup, influenceCircles, tailGroup, goalGroup];
}
