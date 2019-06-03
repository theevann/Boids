/* global currentGroup:true simulation showInfluence */

function createControls(controls) { // eslint-disable-line no-unused-vars
	var controlDiv = d3.select('#main-controls'); //.append('div');

	createGroupControls(controlDiv);
	createSliders(controlDiv, controls);
}

function createSliders (controlDiv, controls) {
	var sliders = controlDiv.append('div')
		.classed('sliders-container', true)
		.selectAll('.slider-container')
		.data(controls)
		.enter()
		.append('div')
		.attr('class', d => d[0])
		.classed('slider-container', true);

	sliders.append('label')
		.classed('slider-label', true)
		.text(d => d[1]);

	sliders.append('input')
		.classed('slider', true)
		.property('type', 'range')
		.property('min', d => d[2])
		.property('max', d => d[3])
		.property('step', d => d[4])
		.property('value', d => simulation.group(currentGroup)[d[0]])
		.on('input', function (d) {
			this.nextSibling.textContent = this.value;
			simulation.group(currentGroup)[d[0]] = +this.value;
		});

	sliders.append('span')
		.classed('slider-value', true)
		.text(d => simulation.group(currentGroup)[d[0]]);

	d3.select('.slider-container.influenceRadius')
		.on('mouseenter', () => simulation.showInfluence = true)
		.on('mouseleave', () => simulation.showInfluence = showInfluence);
}

function createGroupControls(controlDiv) {
	var controlContainerGroup = controlDiv.append('div').classed('control-container-group', true);

	createDropdown(controlContainerGroup);
	updateDropdown();

	controlContainerGroup
		.append('button')
		.classed('add-btn', true)
		.text('Add')
		.on('click', function () {
			simulation.addGroup();
			updateDropdown();
		});

	controlContainerGroup
		.append('button')
		.classed('rmv-btn', true)
		.text('Remove')
		.on('click', function () {
			var next = simulation.nextGroup(currentGroup) || simulation.previousGroup(currentGroup);
			if (next == null) return;
			simulation.removeGroup(currentGroup);
			currentGroup = next.uid;
			updateDropdown();
		});
}

function createDropdown (controlContainerGroup) {
	var dropdownContainerGroup = controlContainerGroup.append('div').classed('dropdown', true);

	dropdownContainerGroup.append('button')
		.classed('drop-btn', true)
		.text('Group ' + currentGroup);

	dropdownContainerGroup.append('div')
		.classed('dropdown-content', true);
}

function updateDropdown() {
	d3.select('.drop-btn')
		.text('Group ' + currentGroup);

	var a = d3.select('.dropdown-content')
		.selectAll('a')
		.data(simulation.groups, g => g.uid);

	a.enter()
		.append('a')
		.text(g => 'Group ' + g.uid)
		.on('click', (group) => {
			currentGroup = group.uid; // Incorrect
			d3.select('.drop-btn').text('Group ' + currentGroup);

			var sliders_container = d3.select('.sliders-container')
				.selectAll('.slider-container');

			sliders_container.select('.slider')
				.property('value', d => simulation.group(currentGroup)[d[0]]);

			sliders_container.select('.slider-value')
				.text(d => simulation.group(currentGroup)[d[0]]);
		});

	a.exit().remove();
}
