prepare-images:
	docker-compose build thesis-deps
	docker-compose build thesis-runtime

build:
	docker pull astaruch/thesis-deps || true
	docker-compose build thesis-builder
	docker pull astaruch/thesis-runtime || true
	docker-compose build thesis-app

run:
	docker run --rm thesis-app

test:
	@echo "TODO: need to implement tests :-)"

push-images:
	docker tag thesis-deps astaruch/thesis-deps
	docker push astaruch/thesis-deps
	docker tag thesis-runtime astaruch/thesis-runtime
	docker push astaruch/thesis-runtime
