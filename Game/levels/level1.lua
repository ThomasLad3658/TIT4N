level1 = {
	entityCount = 2,
	entities = {
		entity1 = {
			tag = "player",
		},
		entity2 = {
			tag = "player",
			x = 50, y = 50,
			OnUpdate = function(self, dt)
				-- Example of custom behavior for this entity
				self.x = self.x + self.speed * dt
			end
		}
	}
}