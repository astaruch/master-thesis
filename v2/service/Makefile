configure:
	docker-compose build thesis-deps

build:
	docker-compose build thesis-builder

run:
	docker-compose build thesis-svc
	docker run --rm thesis/runtime

push-docker-images:
	docker tag thesis/deps astaruch/thesis-deps
	docker push astaruch/thesis-deps
