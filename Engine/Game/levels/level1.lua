level1 = {
	entityCount = 2,
	entities = {
		entity1 = {
			tag = "player",
		},
		entity2 = {
			tag = "player",
			x = 100,
			OnUpdate = function(self, dt)
				-- Example of custom behavior for this entity
				self.x = self.x + self.speed * dt
				--Use setPosition to update the entity's position in the game world
				self.setPosition(self.x, self.y)
			end,
		}
	}
}