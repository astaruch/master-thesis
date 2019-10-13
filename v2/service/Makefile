build:
	docker-compose build thesis-builder
	docker-compose build thesis-runtime

run:
	docker-compose build thesis-app
	docker run --rm thesis-app

test:
	@echo "TODO: need to implement tests :-)"

push-docker-images:
	docker tag thesis-deps astaruch/thesis-deps
	docker push astaruch/thesis-deps
	docker tag thesis-runtime astaruch/thesis-runtime
	docker push astaruch/thesis-runtime
